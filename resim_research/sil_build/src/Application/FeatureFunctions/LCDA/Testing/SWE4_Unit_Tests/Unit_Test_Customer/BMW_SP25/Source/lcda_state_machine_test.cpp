/**
 * @file lcda_state_machine_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lcda_state_machine.c functions
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-70016}
 */

#include "lcda_state_machine_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>


extern "C"
{
#include "fbk_macros.h"
#include "lcda_types.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}

/**
 * Check whether the Transition_From_Not_Available_To_Inactive is as expected
 * \uts{CSCSA-70017} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Not_Available_To_Inactive_bsw)
{
   /** \arrange data required to test transition from not available to inactive state */
   lcda_current_state                                      = LCDA_STATE_NOT_AVAILABLE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc   = FBK_TRUE;
   lcda_input.lcda_input_signals.vehicle_driving_direction = VEHICLE_MOVES_BACKWARD;
   lcda_input.lcda_input_signals.lcda_function_error       = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled      = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw   = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw   = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc   = FBK_ZERO_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Not_Available_To_Inactive is as expected
 * \uts{CSCSA-70018} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Not_Available_To_Inactive_cvw)
{
   /** \arrange data required to test transition from not available to inactive state */
   lcda_current_state                                      = LCDA_STATE_NOT_AVAILABLE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc   = FBK_TRUE;
   lcda_input.lcda_input_signals.vehicle_driving_direction = VEHICLE_MOVES_BACKWARD;
   lcda_input.lcda_input_signals.lcda_function_error       = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled      = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw   = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw   = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc   = FBK_ZERO_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Not_Available_To_Inactive is as expected
 * \uts{CSCSA-70019} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Not_Available_To_Inactive_slc)
{
   /** \arrange data required to test transition from not available to inactive state */
   lcda_current_state                                      = LCDA_STATE_NOT_AVAILABLE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc   = FBK_TRUE;
   lcda_input.lcda_input_signals.vehicle_driving_direction = VEHICLE_MOVES_BACKWARD;
   lcda_input.lcda_input_signals.lcda_function_error       = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled      = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw   = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw   = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc   = FBK_ONE_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Not_Available_To_Inactive is as expected
 * \uts{CSCSA-70020} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Not_Available_To_Available)
{
   /** \arrange data required to test transition from not available to inactive state */
   lcda_current_state                                      = LCDA_STATE_NOT_AVAILABLE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc   = FBK_TRUE;
   lcda_input.lcda_input_signals.vehicle_driving_direction = VEHICLE_MOVES_BACKWARD;
   lcda_input.lcda_input_signals.lcda_function_error       = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw   = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw   = FBK_FALSE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc   = FBK_FALSE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled      = FBK_FALSE;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);


   /** \assert Verify current State is Available */
   EXPECT_NE(lcda_current_state, LCDA_STATE_AVAILABLE);
}

/**
 * check whether the Check Subfunctions are Disabled or not
 * \uts{CSCSA-70021} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test,
       Lcda_Set_Current_Lcda_Functional_State__Transition_From_Not_Available_To_Active_Based_on_Initial_Default_Value_Provided)
{
   /** \arrange data required to test transition from not available to active state */
   lcda_current_state                                           = LCDA_STATE_NOT_AVAILABLE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ZERO_UINT;
   lcda_input.lcda_input_signals.vehicle_condition              = FAHREN;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_FORWARD;
   lcda_input.lcda_input_signals.curve_radii                    = 11.0f;
   lcda_input.lcda_coding_parameters.c_lcda_min_vel_lower_limit = 2.0f;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 11.0f;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 10.0f;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Active */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Available_To_Not_Available
 * \uts{CSCSA-70022} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Available_to_Not_Available)
{
   /** \arrange data required to test transition from not ready to not available state */
   lcda_current_state                                    = LCDA_STATE_AVAILABLE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled    = FBK_FALSE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc = FBK_ZERO_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_NOT_AVAILABLE);
}

/**
 * Check whether the Transition_From_Available_To_Not_Available
 * \uts{CSCSA-70023} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Available_bsw_enabled)
{
   /** \arrange data required to test transition from not ready to not available state */
   lcda_current_state                                    = LCDA_STATE_AVAILABLE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled    = FBK_FALSE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc = FBK_ZERO_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_NOT_AVAILABLE);
}

/**
 * Check whether the Transition_From_Available_To_Not_Available
 * \uts{CSCSA-70024} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Available_cvw_enabled)
{
   /** \arrange data required to test transition from not ready to not available state */
   lcda_current_state                                    = LCDA_STATE_AVAILABLE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled    = FBK_FALSE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc = FBK_ZERO_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_NOT_AVAILABLE);
}

/**
 * Check whether the Transition_From_Available_To_Not_Available
 * \uts{CSCSA-70025} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Available_slc_enabled)
{
   /** \arrange data required to test transition from not ready to not available state */
   lcda_current_state                                    = LCDA_STATE_AVAILABLE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled    = FBK_FALSE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc = FBK_ONE_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_NOT_AVAILABLE);
}

/**
 * Check whether the Transition_From_Available_To_Not_Available
 * \uts{CSCSA-70026} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Available_to_not_available_bsw_cvw_slc)
{
   /** \arrange data required to test transition from not ready to not available state */
   lcda_current_state                                    = LCDA_STATE_AVAILABLE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled    = FBK_FALSE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc = FBK_ONE_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_NOT_AVAILABLE);
}

/**
 * Check whether the Transition_From_Available_To_Not_Available
 * \uts{CSCSA-70027} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Available_to_not_available_lcda_enabled)
{
   /** \arrange data required to test transition from not ready to not available state */
   lcda_current_state                                    = LCDA_STATE_AVAILABLE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled    = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc = FBK_ZERO_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_NOT_AVAILABLE);
}

/**
 * Check whether the Transition_From_Available_To_Inactive
 * \uts{CSCSA-70028} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Available_to_Inactive)
{
   /** \arrange data required to test transition from not ready to inactive state */
   lcda_current_state                                    = LCDA_STATE_AVAILABLE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled    = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc = FBK_ZERO_UINT;

   lcda_input.lcda_input_signals.vehicle_driving_direction = VEHICLE_MOVES_BACKWARD;
   lcda_input.lcda_input_signals.lcda_function_error       = LEVEL0;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Inactive_To_Active
 * \uts{CSCSA-70029} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Inactive_To_Active)
{
   /** \arrange data required to test transition from not inactive to active state */

   lcda_current_state                                           = LCDA_STATE_INACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 3.0f;
   p_vehicle_data->host_speed                                   = 2.0f / LCDA_MPS_2_KPH;
   lcda_input.lcda_input_signals.vehicle_condition              = FAHREN;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_FORWARD;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 1.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 2.5f;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Active */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Inactive_To_Active
 * \uts{CSCSA-70030} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_Inactive_driving_direction)
{
   /** \arrange data required to test transition from not inactive to not active state */

   lcda_current_state                                           = LCDA_STATE_INACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 3.0f;
   p_vehicle_data->host_speed                                   = 2.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = WOHNEN;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_BACKWARD;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 1.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 2.5f;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Active */
   EXPECT_NE(lcda_current_state, LCDA_STATE_ACTIVE);
}

/**
 * Check whether the Transition_not_Inactive_Function_error
 * \uts{CSCSA-70031} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_Inactive_Function_error)
{
   /** \arrange data required to test transition from not inactive to not active state */

   lcda_current_state                                           = LCDA_STATE_INACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 3.0f;
   p_vehicle_data->host_speed                                   = 2.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = PARKENBN_IO;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_FORWARD;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL2;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 1.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 2.5f;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Active */
   EXPECT_NE(lcda_current_state, LCDA_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Inactive_To_Active
 * \uts{CSCSA-70032} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_Inactive_speed)
{
   /** \arrange data required to test transition from not inactive to not active state */

   lcda_current_state                                           = LCDA_STATE_INACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 2.0f;
   p_vehicle_data->host_speed                                   = 3.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = PARKENBN_IO;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_FORWARD;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 1.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 2.5f;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Active */
   EXPECT_NE(lcda_current_state, LCDA_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Inactive_To_Active
 * \uts{CSCSA-70033} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_Inactive_radius)
{
   /** \arrange data required to test transition from not inactive to not active state */

   lcda_current_state                                           = LCDA_STATE_INACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 3.0f;
   p_vehicle_data->host_speed                                   = 2.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = PARKENBN_IO;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_FORWARD;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 2.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 1.5f;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Active */
   EXPECT_NE(lcda_current_state, LCDA_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Inactive_To_Error
 * \uts{CSCSA-70034} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Inactive_To_Error)
{
   /** \arrange data required to test transition from not inactive to error state */

   lcda_current_state                                           = LCDA_STATE_INACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 1.5f;
   p_vehicle_data->host_speed                                   = 2.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = FAHREN;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL2;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 1.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 2.5;
   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Error */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_ERROR);
}

/**
 * Check whether the Transition_From_Inactive_no_change
 * \uts{CSCSA-70035} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_Inactive_function_error)
{
   /** \arrange data required to test transition from not inactive to error state */

   lcda_current_state                                           = LCDA_STATE_INACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 1.5f;
   p_vehicle_data->host_speed                                   = 2.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = FAHREN;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL1;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 1.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 2.5;
   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Error */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Inactive_To_Not_Available
 * \uts{CSCSA-70036} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Inactive_To_Not_Available)
{
   /** \arrange data required to test transition from not inactive to not available state */

   lcda_current_state                                           = LCDA_STATE_INACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 1.5f;
   p_vehicle_data->host_speed                                   = 2.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = FAHREN;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 1.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 2.5f;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_FALSE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ZERO_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_NOT_AVAILABLE);
}

/**
 * Check whether the Transition_From_Inactive_To_Not_Available
 * \uts{CSCSA-70037} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Inactive_To_Not_Available_lcda_enabled)
{
   /** \arrange data required to test transition from not inactive to not available state */

   lcda_current_state                                           = LCDA_STATE_INACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 1.5f;
   p_vehicle_data->host_speed                                   = 2.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = FAHREN;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 1.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 2.5f;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ZERO_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_NOT_AVAILABLE);
}

/**
 * Check whether the Transition_From_Inactive_To_Not_Available
 * \uts{CSCSA-70038} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Inactive_To_Not_Available_bsw_cvw_slc)
{
   /** \arrange data required to test transition from not inactive to not available state */

   lcda_current_state                                           = LCDA_STATE_INACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 1.5f;
   p_vehicle_data->host_speed                                   = 2.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = FAHREN;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 1.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 2.5f;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_FALSE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ONE_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_NOT_AVAILABLE);
}

/**
 * Check whether the Transition_From_Active_To_Inactive
 * \uts{CSCSA-70039} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Active_To_Inactive)
{
   /** \arrange data required to test transition from active to inactive state */
   lcda_current_state                                           = LCDA_STATE_ACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 0.5f;
   p_vehicle_data->host_speed                                   = 1.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = PARKENBN_NIO;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_BACKWARD;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 2.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 1.5f;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Active_To_Inactive
 * \uts{CSCSA-70040} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Active_To_Inactive_driving_direction)
{
   /** \arrange data required to test transition from active to inactive state */
   lcda_current_state                                           = LCDA_STATE_ACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 0.5f;
   p_vehicle_data->host_speed                                   = 1.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = PARKENBN_NIO;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_FORWARD;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 1.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 2.5f;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL1;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ONE_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Active_To_Inactive
 * \uts{CSCSA-70041} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Active_To_Inactive_speed)
{
   /** \arrange data required to test transition from active to inactive state */
   lcda_current_state                                           = LCDA_STATE_ACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 0.5f;
   p_vehicle_data->host_speed                                   = 1.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = PARKENBN_NIO;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_BACKWARD;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 1.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 2.5f;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL1;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ONE_UINT;
   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Active_To_Inactive
 * \uts{CSCSA-70042} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Active_To_Inactive_error)
{
   /** \arrange data required to test transition from active to inactive state */
   lcda_current_state                                           = LCDA_STATE_ACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 0.5f;
   p_vehicle_data->host_speed                                   = 1.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = PARKENBN_IO;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_BACKWARD;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 2.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 1.5f;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL1;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ONE_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Active_To_Not_Available
 * \uts{CSCSA-70043} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Active_To_Not_Available)
{
   /** \arrange data required to test transition from active to not available state */

   lcda_current_state                                           = LCDA_STATE_ACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 0.5f;
   p_vehicle_data->host_speed                                   = 1.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = FAHREN;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_FORWARD;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 2.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 10.0f;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_FALSE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ZERO_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_NOT_AVAILABLE);
}

/**
 * Check whether the Transition_From_Active_To_Not_Available
 * \uts{CSCSA-70044} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Active_To_Not_Available_lcda_enabled)
{
   /** \arrange data required to test transition from active to not available state */

   lcda_current_state                                           = LCDA_STATE_ACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 0.5f;
   p_vehicle_data->host_speed                                   = 1.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = FAHREN;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_FORWARD;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 2.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 10.0f;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ZERO_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_NOT_AVAILABLE);
}

/**
 * Check whether the Transition_From_Active_To_Not_Available
 * \uts{CSCSA-70045} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Active_To_Not_Available_bsw_cvw_slc)
{
   /** \arrange data required to test transition from active to not available state */

   lcda_current_state                                           = LCDA_STATE_ACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 0.5f;
   p_vehicle_data->host_speed                                   = 1.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = FAHREN;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_FORWARD;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 2.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 10.0f;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_FALSE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ONE_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_NOT_AVAILABLE);
}

/**
 * Check whether the Transition_From_Active_To_Not_Available
 * \uts{CSCSA-70046} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Active_To_Not_Available_enabled)
{
   /** \arrange data required to test transition from active to not available state */

   lcda_current_state                                           = LCDA_STATE_ACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 0.5f;
   p_vehicle_data->host_speed                                   = 1.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = FAHREN;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_FORWARD;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 2.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 10.0f;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ONE_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Active_To_Error
 * \uts{CSCSA-70047} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Active_To_Error)
{
   /** \arrange data required to test transition from active to not available state */

   lcda_current_state                                           = LCDA_STATE_ACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 0.5f;
   p_vehicle_data->host_speed                                   = 1.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = WOHNEN;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_FORWARD;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 2.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 10.0f;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL2;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ZERO_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_ERROR);
}

/**
 * Check whether the Transition_From_Active_no_change_function_error
 * \uts{CSCSA-70048} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Active_no_change_function_error)
{
   /** \arrange data required to test transition from active to not available state */

   lcda_current_state                                           = LCDA_STATE_ACTIVE;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 0.5f;
   p_vehicle_data->host_speed                                   = 1.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = WOHNEN;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_FORWARD;
   lcda_input.lcda_coding_parameters.c_min_curve_radii          = 2.5f;
   lcda_input.lcda_input_signals.curve_radii                    = 10.0f;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ZERO_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_ACTIVE);
}

/**
 * Check whether the Transition_From_Error_To_Inactive
 * \uts{CSCSA-70049} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Error_To_Inactive)
{
   /** \arrange data required to test transition from error to inactive state */

   lcda_current_state                                           = LCDA_STATE_ERROR;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 0.5f;
   p_vehicle_data->host_speed                                   = 1.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = PARKENBN_NIO;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_BACKWARD;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ZERO_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Error_To_Inactive_speed
 * \uts{CSCSA-70050} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Error_To_Inactive_speed)
{
   /** \arrange data required to test transition from error to inactive state */

   lcda_current_state                                           = LCDA_STATE_ERROR;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 1.5f;
   p_vehicle_data->host_speed                                   = 0.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = PARKENBN_NIO;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_BACKWARD;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ZERO_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Error_To_Inactive_vehicle_condition
 * \uts{CSCSA-70051} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Error_To_Inactive_vehicle_condition)
{
   /** \arrange data required to test transition from error to inactive state */

   lcda_current_state                                           = LCDA_STATE_ERROR;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 0.5f;
   p_vehicle_data->host_speed                                   = 1.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = FAHREN;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_BACKWARD;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ZERO_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Error_To_Inactive_vehicle_driving_direction
 * \uts{CSCSA-70052} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Error_To_Inactive_vehicle_driving_direction)
{
   /** \arrange data required to test transition from error to inactive state */

   lcda_current_state                                           = LCDA_STATE_ERROR;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 0.5f;
   p_vehicle_data->host_speed                                   = 1.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = PARKENBN_NIO;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_BACKWARD;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ZERO_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_INACTIVE);
}

/**
 * Check whether the Transition_From_Error_To_Inactive_
 * \uts{CSCSA-70053} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Error_To_Inactive_function_error)
{
   /** \arrange data required to test transition from error to inactive state */

   lcda_current_state                                           = LCDA_STATE_ERROR;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 1.5f;
   p_vehicle_data->host_speed                                   = 0.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = FAHREN;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_FORWARD;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL2;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ZERO_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Inactive */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_ERROR);
}

/**
 * check whether the Trasition_From_Error_To_Not_Available
 * \uts{CSCSA-70054} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Error_To_Not_Available)
{
   /** \arrange data required to test transition from error to not available state */
   lcda_current_state                                    = LCDA_STATE_ERROR;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled    = FBK_FALSE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw = FBK_ZERO_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc = FBK_ZERO_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_NOT_AVAILABLE);
}

/**
 * check whether the Trasition_From_Error_To_Not_Available
 * \uts{CSCSA-70055} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Error_To_Not_Available_bsw_cvw_slc)
{
   /** \arrange data required to test transition from error to not available state */
   lcda_current_state                                    = LCDA_STATE_ERROR;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled    = FBK_FALSE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc = FBK_ONE_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_NOT_AVAILABLE);
}

/**
 * check whether the Trasition_From_Error_To_Not_Available
 * \uts{CSCSA-70056} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Error_To_Not_Available_enabled)
{
   /** \arrange data required to test transition from error to not available state */
   lcda_current_state                                    = LCDA_STATE_ERROR;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled    = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc = FBK_ONE_UINT;
   lcda_input.lcda_input_signals.lcda_function_error     = LEVEL1;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_ERROR);
}

/**
 * check whether the Transition_From_Error_To_Active
 * \uts{CSCSA-70057} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Error_To_Active)
{
   /** \arrange data required to test transition from error to Active state */
   lcda_current_state                                           = LCDA_STATE_ERROR;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 3.0f;
   p_vehicle_data->host_speed                                   = 2.0f / LCDA_MPS_2_KPH;
   lcda_input.lcda_input_signals.vehicle_condition              = FAHREN;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_FORWARD;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ONE_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_ACTIVE);
}

/**
 * check whether the Transition_From_Error_To_Active_function_error
 * \uts{CSCSA-70058} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Error_To_Active_error)
{
   /** \arrange data required to test transition from error to Active state */
   lcda_current_state                                           = LCDA_STATE_ERROR;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 2.0f;
   p_vehicle_data->host_speed                                   = 3.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = FAHREN;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_IS_MOVING;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL2;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ONE_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_ERROR);
}

/**
 * check whether the Transition_From_Error_To_Active_speed
 * \uts{CSCSA-70059} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Error_To_Active_speed)
{
   /** \arrange data required to test transition from error to Active state */
   lcda_current_state                                           = LCDA_STATE_ERROR;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 3.0f;
   p_vehicle_data->host_speed                                   = 2.0f / LCDA_MPS_2_KPH;
   lcda_input.lcda_input_signals.vehicle_condition              = FAHREN;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_IS_MOVING;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ONE_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_ERROR);
}

/**
 * check whether the Transition_From_Error_To_Active_driving_direction
 * \uts{CSCSA-70060} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_State_Machine_Test, Lcda_Set_Current_Lcda_Functional_State__Transition_From_Error_To_Active_driving_direction)
{
   /** \arrange data required to test transition from error to Active state */
   lcda_current_state                                           = LCDA_STATE_ERROR;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 2.0f;
   p_vehicle_data->host_speed                                   = 3.0f;
   lcda_input.lcda_input_signals.vehicle_condition              = FAHREN;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_IS_MOVING;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL1;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_cvw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_slc        = FBK_ONE_UINT;

   /** \action Set_Current_Lcda_Functional_State function */
   Lcda_Set_Current_Lcda_Functional_State(&lcda_input, &lcda_current_state, p_vehicle_data);

   /** \assert Verify current State is Not Available */
   EXPECT_EQ(lcda_current_state, LCDA_STATE_ERROR);
}