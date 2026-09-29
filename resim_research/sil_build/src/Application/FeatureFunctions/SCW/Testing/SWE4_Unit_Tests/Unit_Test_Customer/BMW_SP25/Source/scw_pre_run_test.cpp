/**
 * @file scw_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for SCW pre run
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44541}
 */

#include "scw_pre_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "scw_pre_run.c"
#include "scw_types.h"
}


/*
 * Tests filling of guardrail data to scw core input for invalid camera and radar guardrail data.
 * \uts{CSCSA-44542} \sdd{SF-8165} \testtype{negative}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Set_Guardrail_Data__works_properly_for_invalid_guardrail_data)
{
   /** \arrange Set up scw input with invalid guardrail data. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      guardrail_data[idx].f_active                                  = FBK_TRUE;
      guardrail_data[idx].f_present                                 = FBK_FALSE;
      guardrail_data[idx].status                                    = PA_OBJ_STATUS_MATURE;
      p_scw_core_input->guardrail_data[idx].camera.lateral_position = 5.0f;
      p_scw_core_input->guardrail_data[idx].camera.confidence       = 5.0f;
      p_scw_core_input->guardrail_data[idx].camera.type             = SCW_GUARDRAIL_VALID;
   }

   /** \action Call guardrail data setting function. */
   Scw_Set_Guardrail_Data(&scw_instance);

   /** \assert Verify structures are filled with default values. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].camera.lateral_position, 0.0f);
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].camera.confidence, 0.0f);
      EXPECT_EQ(p_scw_core_input->guardrail_data[idx].camera.type, SCW_GUARDRAIL_INVALID);
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].radar.lateral_position, 0.0f);
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].radar.confidence, 0.0f);
      EXPECT_EQ(p_scw_core_input->guardrail_data[idx].radar.type, SCW_GUARDRAIL_INVALID);
   }
}


/*
 * Tests filling of guardrail data to scw core input for invalid camera and radar guardrail data.
 * \uts{CSCSA-101412} \sdd{SF-8165} \testtype{negative}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Set_Guardrail_Data__works_properly_for_invalid_guardrail_data_status)
{
   /** \arrange Set up scw input with invalid guardrail data. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      guardrail_data[idx].f_active                                  = FBK_TRUE;
      guardrail_data[idx].f_present                                 = FBK_FALSE;
      guardrail_data[idx].status                                    = PA_OBJ_STATUS_INVALID;
      p_scw_core_input->guardrail_data[idx].camera.lateral_position = 5.0f;
      p_scw_core_input->guardrail_data[idx].camera.confidence       = 5.0f;
      p_scw_core_input->guardrail_data[idx].camera.type             = SCW_GUARDRAIL_VALID;
   }

   /** \action Call guardrail data setting function. */
   Scw_Set_Guardrail_Data(&scw_instance);

   /** \assert Verify structures are filled with default values. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].camera.lateral_position, 0.0f);
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].camera.confidence, 0.0f);
      EXPECT_EQ(p_scw_core_input->guardrail_data[idx].camera.type, SCW_GUARDRAIL_INVALID);
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].radar.lateral_position, 0.0f);
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].radar.confidence, 0.0f);
      EXPECT_EQ(p_scw_core_input->guardrail_data[idx].radar.type, SCW_GUARDRAIL_INVALID);
   }
}


/*
 * Tests filling of guardrail data to scw core input for valid radar guardrails.
 * \uts{CSCSA-44545} \sdd{SF-8165} \testtype{negative}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Set_Guardrail_Data__works_properly_for_valid_guardrail_data)
{
   /** \arrange Set up scw input with valid radar guardrail data. */
   float32_T pos_expected_value[FBK_NUMBER_OF_SIDES];

   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      pos_expected_value[idx]                   = (8.0f * (float32_T) idx) - 4.0f;
      guardrail_data[idx].f_active              = FBK_TRUE;
      guardrail_data[idx].f_present             = FBK_TRUE;
      guardrail_data[idx].status                = PA_OBJ_STATUS_MATURE;
      guardrail_data[idx].existence_probability = 0.95f;
      guardrail_data[idx].lat_pos               = pos_expected_value[idx];
      guardrail_data[idx].age                   = p_scw_calibration->k_scw_min_guardrail_age + 1u;
   }

   /** \action Call guardrail data setting function. */
   Scw_Set_Guardrail_Data(&scw_instance);

   /** \assert Verify structures are filled with valid radar guardrail values. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].radar.lateral_position, pos_expected_value[idx]);
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].radar.confidence, 0.95f);
      EXPECT_EQ(p_scw_core_input->guardrail_data[idx].radar.type, SCW_GUARDRAIL_VALID);
   }
}


/*
 * Tests filling of guardrail data to scw core input for valid radar guardrails.
 * \uts{CSCSA-101413} \sdd{SF-8165} \testtype{negative}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Set_Guardrail_Data__works_properly_for_valid_guardrail_data_status)
{
   /** \arrange Set up scw input with valid radar guardrail data. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      guardrail_data[idx].f_active              = FBK_TRUE;
      guardrail_data[idx].f_present             = FBK_TRUE;
      guardrail_data[idx].status                = PA_OBJ_STATUS_NEW;
      guardrail_data[idx].existence_probability = 0.95f;
      guardrail_data[idx].lat_pos               = (8.0f * (float32_T) idx) - 4.0f;
      guardrail_data[idx].age                   = p_scw_calibration->k_scw_min_guardrail_age + 1u;
   }

   /** \action Call guardrail data setting function. */
   Scw_Set_Guardrail_Data(&scw_instance);

   /** \assert Verify structures are filled with valid radar guardrail values. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].radar.lateral_position, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].radar.confidence, FBK_ZERO_F);
      EXPECT_EQ(p_scw_core_input->guardrail_data[idx].radar.type, SCW_GUARDRAIL_INVALID);
   }
}


/*
 * Tests filling of guardrail data to scw core input for valid radar guardrails.
 * \uts{CSCSA-101414} \sdd{SF-8165} \testtype{negative}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Set_Guardrail_Data__works_properly_for_valid_guardrail_data_status_coasted)
{
   /** \arrange Set up scw input with valid radar guardrail data. */
   float32_T pos_expected_value[FBK_NUMBER_OF_SIDES];

   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      pos_expected_value[idx]                   = (8.0f * (float32_T) idx) - 4.0f;
      guardrail_data[idx].f_active              = FBK_TRUE;
      guardrail_data[idx].f_present             = FBK_TRUE;
      guardrail_data[idx].status                = PA_OBJ_STATUS_COASTED;
      guardrail_data[idx].existence_probability = 0.95f;
      guardrail_data[idx].lat_pos               = pos_expected_value[idx];
      guardrail_data[idx].age                   = p_scw_calibration->k_scw_min_guardrail_age + 1u;
   }

   /** \action Call guardrail data setting function. */
   Scw_Set_Guardrail_Data(&scw_instance);

   /** \assert Verify structures are filled with valid radar guardrail values. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].radar.lateral_position, pos_expected_value[idx]);
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].radar.confidence, 0.95f);
      EXPECT_EQ(p_scw_core_input->guardrail_data[idx].radar.type, SCW_GUARDRAIL_VALID);
   }
}


/*
 * Tests filling of guardrail data to scw core input for valid radar guardrails.
 * \uts{} \sdd{SF-8165} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Set_Guardrail_Data__age_below_threshold)
{
   /** \arrange Set up scw input with valid radar guardrail data. */
   p_scw_calibration->k_scw_min_guardrail_age = 5u;

   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      guardrail_data[idx].f_active              = FBK_TRUE;
      guardrail_data[idx].f_present             = FBK_TRUE;
      guardrail_data[idx].status                = PA_OBJ_STATUS_COASTED;
      guardrail_data[idx].existence_probability = 0.95f;
      guardrail_data[idx].lat_pos               = (8.0f * (float32_T) idx) - 4.0f;
      guardrail_data[idx].age                   = p_scw_calibration->k_scw_min_guardrail_age - 1u;
   }

   /** \action Call guardrail data setting function. */
   Scw_Set_Guardrail_Data(&scw_instance);

   /** \assert Verify structures are filled with valid radar guardrail values. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      EXPECT_FLOAT_EQ(scw_instance.core_input.guardrail_data[idx].radar.lateral_position, 0.0f);
      EXPECT_FLOAT_EQ(scw_instance.core_input.guardrail_data[idx].radar.confidence, 0.0f);
      EXPECT_EQ(scw_instance.core_input.guardrail_data[idx].radar.type, SCW_GUARDRAIL_INVALID);
   }
}


/*
 * Tests pre run initialization. Expect that default values are returned correctly.
 * \uts{CSCSA-44543} \sdd{SF-8151} \testtype{positive}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Pre_Run_Init__initialize_pre_run_of_function)
{
   /** \arrange Set non default values for scw input. */
   scw_input.f_scw_enable           = 1u;
   scw_input.f_scw_enable_dynamic   = 1u;
   scw_input.f_scw_enable_guardrail = 1u;

   /** \action Call initialization routine for pre run. */
   Scw_Pre_Run_Init(&scw_instance);

   /** \assert Verify structures are filled with default values. */
   EXPECT_EQ(scw_input.f_scw_enable, 1u);
   EXPECT_EQ(scw_input.f_scw_enable_dynamic, 1u);
   EXPECT_EQ(scw_input.f_scw_enable_guardrail, 1u);
}


/*
 * Tests pre run. Expect that in this case all flags are enabled via calibration.
 * \uts{CSCSA-44544} \sdd{SF-8150} \testtype{positive}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Pre_Run__initialize_flags_by_cals)
{
   /** \arrange Set inputs such that flags are enabled via cal. */
   p_scw_calibration->k_scw_f_guardrail_enable_via_cal = FBK_TRUE;
   p_scw_calibration->k_scw_f_dynamic_enable_via_cal   = FBK_TRUE;
   p_scw_calibration->k_scw_f_enable_via_cal           = FBK_TRUE;
   p_scw_core_input->f_scw_enable                      = FBK_FALSE;
   p_scw_core_input->f_scw_enable_dynamic              = FBK_FALSE;
   p_scw_core_input->f_scw_enable_guardrail            = FBK_FALSE;
   scw_input.f_scw_enable_dynamic                      = FBK_ZERO_UINT;
   scw_input.f_scw_enable_guardrail                    = FBK_ZERO_UINT;

   /** \action Call scw pre run. */
   Scw_Pre_Run(&scw_instance, &scw_input, &fbk_output);

   /** \assert Verify structures are filled correctly. */
   EXPECT_EQ(p_scw_core_input->f_scw_enable, 1u);
   EXPECT_EQ(p_scw_core_input->f_scw_enable_dynamic, 1u);
   EXPECT_EQ(p_scw_core_input->f_scw_enable_guardrail, 1u);
}


/*
 * Tests pre run. Expect that in this case all flags are enabled via calibration.
 * \uts{CSCSA-101415} \sdd{SF-8150} \testtype{positive}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Pre_Run__initialize_flags_Active)
{
   /** \arrange Set inputs such that flags are enabled via cal and input. */
   p_scw_calibration->k_scw_f_guardrail_enable_via_cal       = FBK_TRUE;
   p_scw_calibration->k_scw_f_dynamic_enable_via_cal         = FBK_TRUE;
   p_scw_calibration->k_scw_f_enable_via_cal                 = FBK_TRUE;
   p_scw_core_input->f_scw_enable                            = FBK_FALSE;
   p_scw_core_input->f_scw_enable_dynamic                    = FBK_FALSE;
   p_scw_core_input->f_scw_enable_guardrail                  = FBK_FALSE;
   scw_input.f_scw_enable_dynamic                            = FBK_ZERO_UINT;
   scw_input.f_scw_enable_guardrail                          = FBK_ZERO_UINT;
   Scw_Cur_State                                             = SCW_STATE_ACTIVE;
   p_vehicle_data->host_speed                                = 5.0f;
   scw_input.scw_coding_parameters.c_scw_min_vel_lower_limit = (p_vehicle_data->host_speed - 1.0f) * 3.6f;
   scw_input.scw_coding_parameters.c_scw_max_vel_lower_limit = scw_input.scw_coding_parameters.c_scw_min_vel_lower_limit + 1.0f;
   scw_input.scw_coding_parameters.c_scw_max_vel_upper_limit = p_vehicle_data->host_speed * 3.6f - 1.0f;
   scw_input.scw_vehicle_input.vehicle_driving_direction     = SCW_BMW_VEH_MOVING_DIR_MOVES_FORWARD;
   scw_input.scw_vehicle_input.vehicle_condition             = SCW_BMW_PWF_STATE_DRIVING;
   scw_input.scw_vehicle_input.vehicle_dynamometer_status    = SCW_BMW_STATUS_ROLLER_DYNAMOMETER_NO_DYNAMOMETER;
   scw_input.scw_vehicle_input.vehicle_end_of_line_status    = SCW_BMW_STATUS_END_OF_LINE_MODE_NOT_SET;

   /** \action Call scw pre run. */
   Scw_Pre_Run(&scw_instance, &scw_input, &fbk_output);

   /** \assert Verify structures are filled correctly. */
   EXPECT_EQ(p_scw_core_input->f_scw_enable, FBK_TRUE);
   EXPECT_EQ(p_scw_core_input->f_scw_enable_dynamic, FBK_TRUE);
   EXPECT_EQ(p_scw_core_input->f_scw_enable_guardrail, FBK_TRUE);
}


/*
 * Tests pre run. Expect that in this case scw is enabled via state.
 * \uts{} \sdd{SF-8150} \testtype{positive}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Pre_Run__initialize_scw_by_state)
{
   /** \arrange Set inputs such that flags are enabled via cal and input. */
   p_scw_calibration->k_scw_f_guardrail_enable_via_cal       = FBK_FALSE;
   p_scw_calibration->k_scw_f_dynamic_enable_via_cal         = FBK_FALSE;
   p_scw_calibration->k_scw_f_enable_via_cal                 = FBK_FALSE;
   p_scw_core_input->f_scw_enable                            = FBK_FALSE;
   p_scw_core_input->f_scw_enable_dynamic                    = FBK_FALSE;
   p_scw_core_input->f_scw_enable_guardrail                  = FBK_FALSE;
   Scw_Cur_State                                             = SCW_STATE_ACTIVE;
   p_vehicle_data->host_speed                                = 5.0f;
   scw_input.scw_coding_parameters.c_scw_min_vel_lower_limit = (p_vehicle_data->host_speed - 1.0f) * 3.6f;
   scw_input.scw_coding_parameters.c_scw_max_vel_lower_limit = scw_input.scw_coding_parameters.c_scw_min_vel_lower_limit + 1.0f;
   scw_input.scw_coding_parameters.c_scw_max_vel_upper_limit = p_vehicle_data->host_speed * 3.6f + 1.0f;
   scw_input.scw_vehicle_input.vehicle_driving_direction     = SCW_BMW_VEH_MOVING_DIR_MOVES_FORWARD;
   scw_input.scw_vehicle_input.vehicle_condition             = SCW_BMW_PWF_STATE_DRIVING;
   scw_input.scw_vehicle_input.vehicle_dynamometer_status    = SCW_BMW_STATUS_ROLLER_DYNAMOMETER_NO_DYNAMOMETER;
   scw_input.scw_vehicle_input.vehicle_end_of_line_status    = SCW_BMW_STATUS_END_OF_LINE_MODE_NOT_SET;

   /** \action Call scw pre run. */
   Scw_Pre_Run(&scw_instance, &scw_input, &fbk_output);

   /** \assert Verify structures are filled correctly. */
   EXPECT_TRUE(p_scw_core_input->f_scw_enable);
}


/*
 * Tests pre run. Expect that in this case dynamic objects and guardrails are enabled via input.
 * \uts{} \sdd{SF-8150} \testtype{positive}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Pre_Run__initialize_flags_by_input)
{
   /** \arrange Set inputs such that flags are enabled via cal and input. */
   p_scw_calibration->k_scw_f_guardrail_enable_via_cal = FBK_FALSE;
   p_scw_calibration->k_scw_f_dynamic_enable_via_cal   = FBK_FALSE;
   p_scw_core_input->f_scw_enable_dynamic              = FBK_FALSE;
   p_scw_core_input->f_scw_enable_guardrail            = FBK_FALSE;
   scw_input.f_scw_enable_dynamic                      = FBK_ONE_UINT;
   scw_input.f_scw_enable_guardrail                    = FBK_ONE_UINT;

   /** \action Call scw pre run. */
   Scw_Pre_Run(&scw_instance, &scw_input, &fbk_output);

   /** \assert Verify structures are filled correctly. */
   EXPECT_TRUE(p_scw_core_input->f_scw_enable_dynamic);
   EXPECT_TRUE(p_scw_core_input->f_scw_enable_guardrail);
}


/*
 * Tests input initialization. Expect that input is set to default values.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Init_Input__valid_input_pointer)
{
   /** \arrange Set inputs to non default values. */
   scw_input.scw_coding_parameters.c_scw_max_vel_lower_limit = SCW_MAX_VEL_LOWER_LIMIT_DEFAULT - 1.0f;
   scw_input.scw_coding_parameters.c_scw_min_vel_upper_limit = SCW_MIN_VEL_UPPER_LIMIT_DEFAULT + 1.0f;
   scw_input.scw_coding_parameters.country_variant           = SCW_BMW_COUNTRY_VARIANT_EUROPE;
   scw_input.f_scw_enable                                    = FBK_ZERO_UINT;
   scw_input.scw_vehicle_input.vehicle_driving_direction     = SCW_BMW_VEH_MOVING_DIR_MOVES_FORWARD;
   scw_input.scw_vehicle_input.vehicle_condition             = SCW_BMW_PWF_STATE_DRIVING;
   scw_input.scw_vehicle_input.vehicle_dynamometer_status    = SCW_BMW_STATUS_ROLLER_DYNAMOMETER_NO_DYNAMOMETER;
   scw_input.scw_vehicle_input.vehicle_end_of_line_status    = SCW_BMW_STATUS_END_OF_LINE_MODE_NOT_SET;
   scw_input.scw_vehicle_input.vehicle_trailer_status        = SCW_BMW_TRAILER_AVAILABLE;

   /** \action Call scw input init. */
   Scw_Init_Input(&scw_input);

   /** \assert Verify structures are filled correctly. */
   EXPECT_FLOAT_EQ(scw_input.scw_coding_parameters.c_scw_max_vel_lower_limit, SCW_MAX_VEL_LOWER_LIMIT_DEFAULT);
   EXPECT_FLOAT_EQ(scw_input.scw_coding_parameters.c_scw_min_vel_upper_limit, SCW_MIN_VEL_UPPER_LIMIT_DEFAULT);
   EXPECT_EQ(scw_input.scw_coding_parameters.country_variant, SCW_BMW_COUNTRY_VARIANT_NOT_AVAILABLE);
   EXPECT_EQ(scw_input.f_scw_enable, FBK_ONE_UINT);
   EXPECT_EQ(scw_input.scw_vehicle_input.vehicle_driving_direction, SCW_BMW_VEH_MOVING_DIR_UNFILLED);
   EXPECT_EQ(scw_input.scw_vehicle_input.vehicle_condition, SCW_BMW_PWF_STATE_END_DRIVING_AVAILABILITY);
   EXPECT_EQ(scw_input.scw_vehicle_input.vehicle_dynamometer_status, SCW_BMW_STATUS_ROLLER_DYNAMOMETER_SIGNAL_UNFILLED);
   EXPECT_EQ(scw_input.scw_vehicle_input.vehicle_end_of_line_status, SCW_BMW_STATUS_END_OF_LINE_SIGNAL_UNFILLED);
   EXPECT_EQ(scw_input.scw_vehicle_input.vehicle_trailer_status, SCW_BMW_NO_TRAILER_AVAILABLE);
}
