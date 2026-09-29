/**
 * @file scw_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for SCW pre run
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-121176}
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


/**
 * Tests filling of guardrail data to scw core input for invalid camera and radar guardrail data.
 * \uts{CSCSA-124047} \sdd{CSCSA-121179} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Set_Guardrail_Data__works_properly_for_invalid_guardrail_data)
{
   /** \arrange Set up scw input with invalid guardrail data. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      guardrail_data[idx].f_active                                        = FBK_TRUE;
      guardrail_data[idx].f_present                                       = FBK_FALSE;
      guardrail_data[idx].status                                          = PA_OBJ_STATUS_MATURE;
      scw_instance.core_input.guardrail_data[idx].camera.lateral_position = 5.0f;
      scw_instance.core_input.guardrail_data[idx].camera.confidence       = 5.0f;
      scw_instance.core_input.guardrail_data[idx].camera.type             = SCW_GUARDRAIL_VALID;
   }

   /** \action Call guardrail data setting function. */
   Scw_Set_Guardrail_Data(&scw_instance);

   /** \assert Verify structures are filled with default values. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      EXPECT_FLOAT_EQ(scw_instance.core_input.guardrail_data[idx].camera.lateral_position, 0.0f);
      EXPECT_FLOAT_EQ(scw_instance.core_input.guardrail_data[idx].camera.confidence, 0.0f);
      EXPECT_EQ(scw_instance.core_input.guardrail_data[idx].camera.type, SCW_GUARDRAIL_INVALID);
      EXPECT_FLOAT_EQ(scw_instance.core_input.guardrail_data[idx].radar.lateral_position, 0.0f);
      EXPECT_FLOAT_EQ(scw_instance.core_input.guardrail_data[idx].radar.confidence, 0.0f);
      EXPECT_EQ(scw_instance.core_input.guardrail_data[idx].radar.type, SCW_GUARDRAIL_INVALID);
   }
}


/*
 * Tests filling of guardrail data to scw core input for invalid camera and radar guardrail data.
 * \uts{CSCSA-124045} \sdd{CSCSA-121179} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Set_Guardrail_Data__works_properly_for_invalid_guardrail_data_status)
{
   /** \arrange Set up scw input with invalid guardrail data. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      guardrail_data[idx].f_active                                        = FBK_TRUE;
      guardrail_data[idx].f_present                                       = FBK_FALSE;
      guardrail_data[idx].status                                          = PA_OBJ_STATUS_INVALID;
      scw_instance.core_input.guardrail_data[idx].camera.lateral_position = 5.0f;
      scw_instance.core_input.guardrail_data[idx].camera.confidence       = 5.0f;
      scw_instance.core_input.guardrail_data[idx].camera.type             = SCW_GUARDRAIL_VALID;
   }

   /** \action Call guardrail data setting function. */
   Scw_Set_Guardrail_Data(&scw_instance);

   /** \assert Verify structures are filled with default values. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      EXPECT_FLOAT_EQ(scw_instance.core_input.guardrail_data[idx].camera.lateral_position, 0.0f);
      EXPECT_FLOAT_EQ(scw_instance.core_input.guardrail_data[idx].camera.confidence, 0.0f);
      EXPECT_EQ(scw_instance.core_input.guardrail_data[idx].camera.type, SCW_GUARDRAIL_INVALID);
      EXPECT_FLOAT_EQ(scw_instance.core_input.guardrail_data[idx].radar.lateral_position, 0.0f);
      EXPECT_FLOAT_EQ(scw_instance.core_input.guardrail_data[idx].radar.confidence, 0.0f);
      EXPECT_EQ(scw_instance.core_input.guardrail_data[idx].radar.type, SCW_GUARDRAIL_INVALID);
   }
}


/*
 * Tests filling of guardrail data to scw core input for valid radar guardrails.
 * \uts{CSCSA-124046} \sdd{CSCSA-121179} \testtype{SoftwareUpdateTesting}
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
 * \uts{CSCSA-124050} \sdd{CSCSA-121179} \testtype{SoftwareUpdateTesting}
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
      EXPECT_FLOAT_EQ(scw_instance.core_input.guardrail_data[idx].radar.lateral_position, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_instance.core_input.guardrail_data[idx].radar.confidence, FBK_ZERO_F);
      EXPECT_EQ(scw_instance.core_input.guardrail_data[idx].radar.type, SCW_GUARDRAIL_INVALID);
   }
}


/*
 * Tests filling of guardrail data to scw core input for valid radar guardrails.
 * \uts{CSCSA-124051} \sdd{CSCSA-121179} \testtype{SoftwareUpdateTesting}
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
      EXPECT_FLOAT_EQ(scw_instance.core_input.guardrail_data[idx].radar.lateral_position, pos_expected_value[idx]);
      EXPECT_FLOAT_EQ(scw_instance.core_input.guardrail_data[idx].radar.confidence, 0.95f);
      EXPECT_EQ(scw_instance.core_input.guardrail_data[idx].radar.type, SCW_GUARDRAIL_VALID);
   }
}


/*
 * Tests filling of guardrail data to scw core input for valid radar guardrails.
 * \uts{CSCSA-290695} \sdd{CSCSA-121179} \testtype{SoftwareUpdateTesting}
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
 * \uts{CSCSA-124048} \sdd{CSCSA-196754} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Init_Input__initialize_pre_run_of_function)
{
   /** \arrange Set non default values for scw input. */
   scw_input.f_scw_enable           = 0u;
   scw_input.f_scw_enable_dynamic   = 0u;
   scw_input.f_scw_enable_guardrail = 0u;

   /** \action Call initialization routine for pre run. */
   Scw_Init_Input(&scw_input);

   /** \assert Verify structures are filled with default values. */
   EXPECT_EQ(scw_input.f_scw_enable, 1u);
   EXPECT_EQ(scw_input.f_scw_enable_dynamic, 1u);
   EXPECT_EQ(scw_input.f_scw_enable_guardrail, 1u);
}

#ifndef NDEBUG
/*
 * Tests pre run initialization. Expect that exception is thrown.
 * \uts{CSCSA-185980} \sdd{SF-8151} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Pre_Run_Init__invalid_instance_pointer)
{
   /** \arrange */
   /** \action Call initialization routine for pre run. */
   /** \assert Verify that exception is thrown. */
   EXPECT_DEATH({ Scw_Pre_Run_Init(NULL); }, ".*p_scw_instance.*");
}
#endif //! NDEBUG

/*
 * Tests input init. Expect that in this case all input flags are enabled.
 * \uts{CSCSA-185981} \sdd{CSCSA-196754} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Init_Input__valid_input_pointer)
{
   /** \arrange Set inputs such that flags are disabled. */
   scw_input.f_scw_enable           = FBK_FALSE;
   scw_input.f_scw_enable_dynamic   = FBK_FALSE;
   scw_input.f_scw_enable_guardrail = FBK_FALSE;

   /** \action Call scw input init. */
   Scw_Init_Input(&scw_input);

   /** \assert Verify all flags enabled. */
   EXPECT_TRUE(scw_input.f_scw_enable);
   EXPECT_TRUE(scw_input.f_scw_enable_dynamic);
   EXPECT_TRUE(scw_input.f_scw_enable_guardrail);
}


/*
 * Tests pre run. Expect that in this case all flags are enabled via input.
 * \uts{CSCSA-124049} \sdd{SF-8150} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Pre_Run__initialize_flags_Active)
{
   /** \arrange Set inputs such that flags are enabled via input. */
   scw_input.f_scw_enable                         = FBK_TRUE;
   scw_input.f_scw_enable_dynamic                 = FBK_TRUE;
   scw_input.f_scw_enable_guardrail               = FBK_TRUE;
   scw_instance.core_input.f_scw_enable           = FBK_FALSE;
   scw_instance.core_input.f_scw_enable_dynamic   = FBK_FALSE;
   scw_instance.core_input.f_scw_enable_guardrail = FBK_FALSE;

   /** \action Call scw pre run. */
   Scw_Pre_Run(&scw_instance, &scw_input, &fbk_output);

   /** \assert Verify structures are filled correctly. */
   EXPECT_EQ(scw_instance.core_input.f_scw_enable, FBK_TRUE);
   EXPECT_EQ(scw_instance.core_input.f_scw_enable_dynamic, FBK_TRUE);
   EXPECT_EQ(scw_instance.core_input.f_scw_enable_guardrail, FBK_TRUE);
}


/*
 * Tests filling of guardrail data to scw core input for valid radar guardrails.
 * \uts{CSCSA-290696} \sdd{CSCSA-290616} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Set_Guardrail_Data_Valid__nonzero_previous_lateral_position)
{
   /** \arrange Set up scw input with valid radar guardrail data. */
   float32_T pos_expected_value[FBK_NUMBER_OF_SIDES];

   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      pos_expected_value[idx]                   = (8.0f * (float32_T) idx) - 4.0f;
      guardrail_data[idx].existence_probability = 0.95f;
      guardrail_data[idx].lat_pos               = pos_expected_value[idx];
      p_scw_persistent->grail_lat_position[idx] = pos_expected_value[idx];
   }

   /** \action Call guardrail data setting function. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      Scw_Set_Guardrail_Data_Valid(&scw_instance, &(guardrail_data[idx]), idx);
   }

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
 * \uts{CSCSA-290697} \sdd{CSCSA-290616} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Set_Guardrail_Data_Valid__nonzero_previous_lateral_position_lower_than_EPSILON)
{
   /** \arrange Set up scw input with valid radar guardrail data. */
   float32_T pos_expected_value[FBK_NUMBER_OF_SIDES];
   p_scw_calibration->k_scw_max_lat_pos_ratio = 0.5f;

   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      pos_expected_value[idx]                     = (8.0f * (float32_T) idx) - 4.0f;
      guardrail_data[idx].existence_probability   = 0.95f;
      guardrail_data[idx].lat_pos                 = pos_expected_value[idx];
      p_scw_persistent->grail_lat_position[idx]   = EPSILON * (((float32_T) idx) - 0.5f);
      p_scw_persistent->grail_freeze_counter[idx] = 0u;
   }

   /** \action Call guardrail data setting function. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      Scw_Set_Guardrail_Data_Valid(&scw_instance, &(guardrail_data[idx]), idx);
   }

   /** \assert Verify structures are filled with valid radar guardrail values. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].radar.lateral_position, pos_expected_value[idx]);
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].radar.confidence, 0.95f);
      EXPECT_EQ(p_scw_core_input->guardrail_data[idx].radar.type, SCW_GUARDRAIL_VALID);
      EXPECT_EQ(p_scw_persistent->grail_freeze_counter[idx], 0u);
   }
}


/*
 * Tests filling of guardrail data to scw core input for valid radar guardrails.
 * \uts{CSCSA-290698} \sdd{CSCSA-290616} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Set_Guardrail_Data_Valid__nonzero_freeze_counter)
{
   /** \arrange Set up scw input with valid radar guardrail data. */
   float32_T pos_expected_value[FBK_NUMBER_OF_SIDES];
   p_scw_calibration->k_scw_max_lat_pos_ratio = 0.5f;

   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      pos_expected_value[idx]                     = (8.0f * (float32_T) idx) - 4.0f;
      guardrail_data[idx].existence_probability   = 0.95f;
      guardrail_data[idx].lat_pos                 = (p_scw_calibration->k_scw_max_lat_pos_ratio - 0.1f) * pos_expected_value[idx];
      p_scw_persistent->grail_lat_position[idx]   = pos_expected_value[idx];
      p_scw_persistent->grail_freeze_counter[idx] = 1u;
   }

   /** \action Call guardrail data setting function. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      Scw_Set_Guardrail_Data_Valid(&scw_instance, &(guardrail_data[idx]), idx);
   }

   /** \assert Verify structures are filled with valid radar guardrail values. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].radar.lateral_position, pos_expected_value[idx]);
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].radar.confidence, 0.0f);
      EXPECT_EQ(p_scw_core_input->guardrail_data[idx].radar.type, SCW_GUARDRAIL_VALID);
      EXPECT_EQ(p_scw_persistent->grail_freeze_counter[idx], 0u);
   }
}


/*
 * Tests filling of guardrail data to scw core input for valid radar guardrails.
 * \uts{CSCSA-290699} \sdd{CSCSA-290616} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Pre_Run_Test, Scw_Set_Guardrail_Data_Valid__lateral_position_ratio_below_the_threshold)
{
   /** \arrange Set up scw input with valid radar guardrail data. */
   float32_T pos_expected_value[FBK_NUMBER_OF_SIDES];
   p_scw_calibration->k_scw_max_lat_pos_ratio = 0.5f;

   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      pos_expected_value[idx]                     = (8.0f * (float32_T) idx) - 4.0f;
      guardrail_data[idx].existence_probability   = 0.95f;
      guardrail_data[idx].lat_pos                 = (p_scw_calibration->k_scw_max_lat_pos_ratio - 0.1f) * pos_expected_value[idx];
      p_scw_persistent->grail_lat_position[idx]   = pos_expected_value[idx];
      p_scw_persistent->grail_freeze_counter[idx] = 0u;
   }

   /** \action Call guardrail data setting function. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      Scw_Set_Guardrail_Data_Valid(&scw_instance, &(guardrail_data[idx]), idx);
   }

   /** \assert Verify structures are filled with valid radar guardrail values. */
   for (uint8_t idx = 0; idx < FBK_NUMBER_OF_SIDES; idx++)
   {
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].radar.lateral_position, pos_expected_value[idx]);
      EXPECT_FLOAT_EQ(p_scw_core_input->guardrail_data[idx].radar.confidence, 0.0f);
      EXPECT_EQ(p_scw_core_input->guardrail_data[idx].radar.type, SCW_GUARDRAIL_VALID);
      EXPECT_EQ(p_scw_persistent->grail_freeze_counter[idx], (p_scw_calibration->k_scw_guardrail_freeze_period - 1u));
   }
}
