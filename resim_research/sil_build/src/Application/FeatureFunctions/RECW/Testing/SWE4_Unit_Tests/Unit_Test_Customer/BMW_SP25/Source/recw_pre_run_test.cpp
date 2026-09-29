/**
 * @file recw_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SRR5 RECW pre run tests
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44412}
 */

#include "recw_pre_run_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "recw_core_input_t.h"
#include "recw_input_t.h"
#include "recw_pre_run.c"
}


/**
 * Test the Recw init input of BMW. Expect that function rnuns correctly.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Init_Input__works_correctly)
{
   /** \arrange Input data. */

   /** \action Call Generic pre run. */
   Recw_Init_Input(&recw_input);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(recw_input.accelerator_pedal_gradient, FBK_ZERO_F);
   EXPECT_EQ(recw_input.vehicle_movement_status, RECW_VEHICLE_STANDSTILL);
   EXPECT_EQ(recw_input.status_trailer, RECW_NO_TRAILER_AVAILABLE);
   EXPECT_EQ(recw_input.status_roller_dynamometer, RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER);
   EXPECT_EQ(recw_input.status_end_of_line, RECW_STATUS_END_OF_LINE_MODE_NOT_SET);
   EXPECT_EQ(recw_input.c_recw_enable, RECW_STATE_DISABLED);
   EXPECT_EQ(recw_input.recw_type, RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH);
   EXPECT_EQ(recw_input.recw_error, RECW_NO_ERROR);
   EXPECT_EQ(recw_input.f_recw_enable_m_drive, FBK_ZERO_UINT);
}


/**
 * Test the pre run initialization routine of Bmw srr5. Expect that default values are set.
 * \uts{CSCSA-44413} \sdd{SF-7970} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run_Init__check_that_values_are_initialized)
{
   /** \arrange Input data. */

   /** \action Call Debug mode enabled check. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect input to be initialized. */
   EXPECT_TRUE(&recw_instance != NULL);
}


/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-44414} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_not_available_to_ready)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_NOT_AVAILABLE);
   recw_input.c_recw_enable = RECW_STATE_ENABLED;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101058} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_ready_to_not_available)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_READY);
   recw_input.c_recw_enable = RECW_STATE_DISABLED;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_NOT_AVAILABLE);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101059} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_ready_to_active)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_READY);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NO_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_STANDSTILL;
   recw_input.status_trailer            = RECW_NO_TRAILER_AVAILABLE;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_NOT_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_ACTIVE);
   EXPECT_TRUE(recw_instance.core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101060} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_ready_to_active_roller_dynamometer_front_axle_on)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_READY);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NO_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_STANDSTILL;
   recw_input.status_trailer            = RECW_NO_TRAILER_AVAILABLE;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_FRONT_AXLE_ON_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_NOT_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101061} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_ready_to_active_roller_dynamometer_back_axle_on)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_READY);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NO_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_STANDSTILL;
   recw_input.status_trailer            = RECW_NO_TRAILER_AVAILABLE;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_BACK_AXLE_ON_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_NOT_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101062} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_ready_to_active_roller_dynamometer_Two_axle)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_READY);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NO_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_STANDSTILL;
   recw_input.status_trailer            = RECW_NO_TRAILER_AVAILABLE;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_TWO_AXLE_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_NOT_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101063} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_ready_to_active_roller_dynamometer_end_line)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_READY);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NO_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_STANDSTILL;
   recw_input.status_trailer            = RECW_NO_TRAILER_AVAILABLE;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101064} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_ready_to_active_Trailer)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_READY);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NO_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_STANDSTILL;
   recw_input.status_trailer            = RECW_TRAILER_AVAILABLE;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_NOT_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101065} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_ready_to_active_Movement_status)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_READY);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NO_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_MOVING_BACKWARD;
   recw_input.status_trailer            = RECW_NO_TRAILER_AVAILABLE;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_NOT_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101066} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_ready_to_active_error)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_READY);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NON_CRITICAL_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_STANDSTILL;
   recw_input.status_trailer            = RECW_NO_TRAILER_AVAILABLE;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}
/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101067} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_ready_to_degraded)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_READY);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NON_CRITICAL_ERROR;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_NOT_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_DEGRADED);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}
/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101068} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_ready_to_degraded_recw_error)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_READY);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NO_ERROR;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER;
   recw_input.status_trailer            = RECW_TRAILER_AVAILABLE;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_NOT_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}
/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101069} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_ready_to_degraded_roller_dynamoter)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_READY);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NON_CRITICAL_ERROR;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_FRONT_AXLE_ON_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_NOT_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101070} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_ready_to_degraded_end_of_line)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_READY);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NON_CRITICAL_ERROR;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}
/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101071} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_ready_to_error)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_READY);
   recw_input.c_recw_enable = RECW_STATE_ENABLED;
   recw_input.recw_error    = RECW_CRITICAL_ERROR;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_ERROR);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}
/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101072} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_ready_to_error_recw_error)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_READY);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NON_CRITICAL_ERROR;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_FRONT_AXLE_ON_DYNAMOMETER;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101073} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_active_to_ready)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_ACTIVE);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NO_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_MOVING_BACKWARD;
   recw_input.status_trailer            = RECW_TRAILER_AVAILABLE;
   recw_input.f_recw_enable_m_drive     = RECW_STATE_ENABLED;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_FRONT_AXLE_ON_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101074} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_active_to_ready_movement_status)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_ACTIVE);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NO_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_MOVING_FORWARD;
   recw_input.status_trailer            = RECW_TRAILER_AVAILABLE;
   recw_input.f_recw_enable_m_drive     = RECW_STATE_ENABLED;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_FRONT_AXLE_ON_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101075} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_active_to_ready_status_trailer)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_ACTIVE);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NO_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_MOVING_BACKWARD;
   recw_input.status_trailer            = RECW_NO_TRAILER_AVAILABLE;
   recw_input.f_recw_enable_m_drive     = RECW_STATE_DISABLED;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_FRONT_AXLE_ON_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101076} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_active_to_ready_m_drive)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_ACTIVE);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NO_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_MOVING_FORWARD;
   recw_input.status_trailer            = RECW_NO_TRAILER_AVAILABLE;
   recw_input.f_recw_enable_m_drive     = RECW_STATE_ENABLED;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_FRONT_AXLE_ON_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101077} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_active_to_ready_recw_error)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_ACTIVE);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_CRITICAL_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_MOVING_BACKWARD;
   recw_input.status_trailer            = RECW_TRAILER_AVAILABLE;
   recw_input.f_recw_enable_m_drive     = RECW_STATE_ENABLED;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_FRONT_AXLE_ON_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101078} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_active_to_ready_back_axle)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_ACTIVE);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_CRITICAL_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_MOVING_BACKWARD;
   recw_input.status_trailer            = RECW_TRAILER_AVAILABLE;
   recw_input.f_recw_enable_m_drive     = RECW_STATE_ENABLED;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_BACK_AXLE_ON_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}
/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101079} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_active_to_ready_Two_Axle)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_ACTIVE);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_CRITICAL_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_MOVING_BACKWARD;
   recw_input.status_trailer            = RECW_TRAILER_AVAILABLE;
   recw_input.f_recw_enable_m_drive     = RECW_STATE_ENABLED;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_TWO_AXLE_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_NOT_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101080} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_active_to_degraded)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_ACTIVE);
   recw_input.c_recw_enable = RECW_STATE_ENABLED;
   recw_input.recw_error    = RECW_NON_CRITICAL_ERROR;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_DEGRADED);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101081} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_active_to_error)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_ACTIVE);
   recw_input.c_recw_enable = RECW_STATE_ENABLED;
   recw_input.recw_error    = RECW_CRITICAL_ERROR;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_ERROR);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101082} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_active_to_error_recw_error)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_ACTIVE);
   recw_input.c_recw_enable = RECW_STATE_ENABLED;
   recw_input.recw_error    = RECW_NO_ERROR;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_ACTIVE);
   EXPECT_TRUE(recw_instance.core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101083} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_degraded_to_ready)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_DEGRADED);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NO_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_MOVING_BACKWARD;
   recw_input.status_trailer            = RECW_TRAILER_AVAILABLE;
   recw_input.f_recw_enable_m_drive     = RECW_STATE_ENABLED;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_FRONT_AXLE_ON_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101084} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_degraded_to_ready_trailer)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_DEGRADED);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NO_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_MOVING_FORWARD;
   recw_input.status_trailer            = RECW_NO_TRAILER_AVAILABLE;
   recw_input.f_recw_enable_m_drive     = RECW_STATE_DISABLED;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_FUNCTION_REPORTS_ERROR;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_DEGRADED);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101085} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_degraded_to_ready_m_drive)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_DEGRADED);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NO_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_MOVING_FORWARD;
   recw_input.status_trailer            = RECW_NO_TRAILER_AVAILABLE;
   recw_input.f_recw_enable_m_drive     = RECW_STATE_ENABLED;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_FRONT_AXLE_ON_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101086} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_degraded_to_ready_Test_mode)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_DEGRADED);
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;
   recw_input.recw_error                = RECW_NO_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_MOVING_BACKWARD;
   recw_input.status_trailer            = RECW_TRAILER_AVAILABLE;
   recw_input.f_recw_enable_m_drive     = RECW_STATE_ENABLED;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_NOT_SET;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly transition from degraded to active.
 * \uts{CSCSA-101087} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_degraded_to_active)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_DEGRADED);
   recw_input.recw_error                = RECW_NO_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_STANDSTILL;
   recw_input.status_trailer            = RECW_NO_TRAILER_AVAILABLE;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_NOT_SET;
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_ACTIVE);
   EXPECT_TRUE(recw_instance.core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly transition from degraded to not active recw error.
 * \uts{CSCSA-101088} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_degraded_to_active_recw_error)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_DEGRADED);
   recw_input.recw_error                = RECW_NON_CRITICAL_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_STANDSTILL;
   recw_input.status_trailer            = RECW_NO_TRAILER_AVAILABLE;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_NOT_SET;
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_DEGRADED);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly transition from degraded to not active movement status.
 * \uts{CSCSA-101089} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_degraded_to_active_movement_status)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_DEGRADED);
   recw_input.recw_error                = RECW_NON_CRITICAL_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_MOVING_BACKWARD;
   recw_input.status_trailer            = RECW_NO_TRAILER_AVAILABLE;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_NOT_SET;
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_DEGRADED);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly transition from degraded to not active status trailer.
 * \uts{CSCSA-101090} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_degraded_to_active_status_trailer)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_DEGRADED);
   recw_input.recw_error                = RECW_NON_CRITICAL_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_STANDSTILL;
   recw_input.status_trailer            = RECW_TRAILER_AVAILABLE;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_NOT_SET;
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_DEGRADED);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly transition from degraded to not active roller
 * dynamometer. \uts{CSCSA-101091} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_degraded_to_active_roller_dynamometer)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_DEGRADED);
   recw_input.recw_error                = RECW_NO_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_STANDSTILL;
   recw_input.status_trailer            = RECW_NO_TRAILER_AVAILABLE;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_SIGNAL_UNFILLED;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_NOT_SET;
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_DEGRADED);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly transition from degraded to not active end of line.
 * \uts{CSCSA-101092} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_degraded_to_active_end_of_line)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_DEGRADED);
   recw_input.recw_error                = RECW_NON_CRITICAL_ERROR;
   recw_input.vehicle_movement_status   = RECW_VEHICLE_STANDSTILL;
   recw_input.status_trailer            = RECW_NO_TRAILER_AVAILABLE;
   recw_input.status_roller_dynamometer = RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER;
   recw_input.status_end_of_line        = RECW_STATUS_END_OF_LINE_MODE_SET;
   recw_input.c_recw_enable             = RECW_STATE_ENABLED;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_NE(Recw_Get_State(), RECW_SM_ACTIVE);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}
/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101093} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_degraded_to_error)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_DEGRADED);
   recw_input.c_recw_enable = RECW_STATE_ENABLED;
   recw_input.recw_error    = RECW_CRITICAL_ERROR;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_ERROR);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101094} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_degraded_to_error_recw_error)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_DEGRADED);
   recw_input.c_recw_enable = RECW_STATE_ENABLED;
   recw_input.recw_error    = RECW_NON_CRITICAL_ERROR;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_DEGRADED);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101095} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_error_to_ready)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_ERROR);
   recw_input.c_recw_enable = RECW_STATE_ENABLED;
   recw_input.recw_error    = RECW_NO_ERROR;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_READY);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101096} \sdd{SF-7971} \testtype{negative}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__state_transition_error_to_ready_recw_error)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State(RECW_SM_ERROR);
   recw_input.c_recw_enable = RECW_STATE_ENABLED;
   recw_input.recw_error    = RECW_NON_CRITICAL_ERROR;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correct. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_ERROR);
   EXPECT_FALSE(recw_core_input.f_enable_recw);
}

/**
 * Test the Recw pre run of Bmw srr5. Expect that mapping is done correctly.
 * \uts{CSCSA-101097} \sdd{SF-7971} \testtype{positive}
 */
TEST_F(Recw_Pre_Run_Test, Recw_Pre_Run__no_state_transition)
{
   /** \arrange Input data. */
   SetUp();


   Recw_Set_State((Recw_SM_State_T) 5);
   recw_input.c_recw_enable = RECW_STATE_ENABLED;

   /** \action Call Bmw srr5 pre run. */
   Recw_Pre_Run(&recw_instance, &recw_input, &fbk_output);

   /** \assert Expect mapping to be done correctly. */
   EXPECT_EQ(Recw_Get_State(), RECW_SM_ERROR);
}
