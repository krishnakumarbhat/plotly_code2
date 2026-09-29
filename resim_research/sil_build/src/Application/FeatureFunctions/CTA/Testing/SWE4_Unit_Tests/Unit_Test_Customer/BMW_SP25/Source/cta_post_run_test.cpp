/**
 * @file cta_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SRR5 CTA post run
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42082}
 */

#include "cta_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "cta_types.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}


/**
 * Check whether the final deceleration value is returned directly when there is no partially increment from cycle to cycle.
 * \uts{CSCSA-42083} \sdd{SF-3962} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Braking_Acceleration__Decelerate_Host_Vehicle_With_Maximum_Deceleration_Value)
{
   /** \arrange result acceleration */
   float32_T result_acceleration;
   p_cta_cals->k_ctb_time_to_ask_for_final_brake_decel = 0.0f;

   /** \action Test the brake deceleration function */
   result_acceleration = Cta_Get_Braking_Acceleration(fbk_output.p_pa_data, p_cta_cals);

   /** \assert check whether final value is directly returned. */
   EXPECT_FLOAT_EQ(result_acceleration, -p_cta_cals->k_ctb_const_decel_after_ramp_in);
}


/**
 * Check whether the an incremented time is returned after each cycle.
 * \uts{CSCSA-42084} \sdd{SF-3962} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Braking_Acceleration__Decelerate_Host_Vehicle_Incrementally)
{
   /** \arrange Reset persistent postrun data and calculate the expected values */
   Current_Ctb_Brake_Deceleration  = 0.0f;
   const uint8_t amount_iterations = 3;
   float32_T incremented_decel;
   p_cta_cals->k_ctb_time_to_ask_for_final_brake_decel = 0.15f;
   p_cta_cals->k_ctb_const_decel_after_ramp_in         = 6.0f;
   incremented_decel = -p_cta_cals->k_ctb_const_decel_after_ramp_in / (p_cta_cals->k_ctb_time_to_ask_for_final_brake_decel / 0.05f);
   float32_T expected_acceleration[amount_iterations] = {incremented_decel, 2.0f * incremented_decel, 3.0f * incremented_decel};

   /** \action Test the brake deceleration function */
   for (uint8_t i = 0; i < amount_iterations; i++)
   {
      Current_Ctb_Brake_Deceleration = Cta_Get_Braking_Acceleration(fbk_output.p_pa_data, p_cta_cals);

      /** \assert check returned value is equal to the incremented precalculated value. */
      EXPECT_FLOAT_EQ(Current_Ctb_Brake_Deceleration, expected_acceleration[i]);
   }
}

/**
 * Check whether the flag is set up correctly. Here all conditions for a brake request are fulfilled, thus true is expected.
 * \uts{CSCSA-42085} \sdd{SF-3963} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Check_Rctb_Qualifier__all_conditions_are_met_for_setting_of_a_brake_qualifier)
{
   /** \arrange Set up inputs such that true is returned. */
   boolean_T res;
   uint8_t approach_side                               = FBK_SIDE_LEFT;
   uint8_t ctb_mode                                    = CTA_MODE_REAR;
   p_cta_cals->k_ctb_time_to_ask_for_final_brake_decel = 0.0f;

   cta_instance.core_output.cta_alert_level[ctb_mode][approach_side]   = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.f_brake_qualifier[ctb_mode][approach_side] = FBK_TRUE;

   /** \action Test the correct returning of the qualifier. */
   res = Cta_Check_Rctb_Qualifier(&cta_instance.core_output, approach_side, ctb_mode);

   /** \assert expect that the qualifier is set to true. */
   EXPECT_TRUE(res);
}

/**
 * Check whether the flag is set up correctly. Here FCTA is enabled, thus false is expected.
 * \uts{CSCSA-42086} \sdd{SF-3963} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Check_Rctb_Qualifier__false_qualifier_is_expected_since_fcta_is_enabled)
{
   /** \arrange Set up inputs such that false is returned. */
   boolean_T res;
   uint8_t approach_side                               = FBK_SIDE_LEFT;
   uint8_t ctb_mode                                    = CTA_MODE_REAR;
   p_cta_cals->k_ctb_time_to_ask_for_final_brake_decel = 0.0f;

   cta_instance.core_output.cta_alert_level[ctb_mode][approach_side]   = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.f_brake_qualifier[ctb_mode][approach_side] = FBK_TRUE;

   /** \action Test the correct returning of the qualifier. */
   res = Cta_Check_Rctb_Qualifier(&cta_instance.core_output, approach_side, ctb_mode);

   /** \assert expect that the qualifier is set to false. */
   EXPECT_TRUE(res);
}

/**
 * Check whether the flag is set up correctly. Here no alert is given, thus false is expected.
 * \uts{CSCSA-42087} \sdd{SF-3963} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Check_Rctb_Qualifier__false_qualifier_is_expected_since_no_alert_is_given)
{
   /** \arrange Set up inputs such that false is returned. */
   boolean_T res;
   uint8_t approach_side                               = FBK_SIDE_LEFT;
   uint8_t ctb_mode                                    = CTA_MODE_REAR;
   p_cta_cals->k_ctb_time_to_ask_for_final_brake_decel = 0.0f;

   cta_instance.core_output.cta_alert_level[ctb_mode][approach_side]   = CTA_CRIT_LEVEL_1;
   cta_instance.core_output.f_brake_qualifier[ctb_mode][approach_side] = FBK_TRUE;

   /** \action Test the correct returning of the qualifier. */
   res = Cta_Check_Rctb_Qualifier(&cta_instance.core_output, approach_side, ctb_mode);

   /** \assert expect that the qualifier is set to false due to missing alert. */
   EXPECT_FALSE(res);
}


/**
 * Check whether the flag is set up correctly. Here the core is not putting out a brake qualifier, thus false is expected.
 * \uts{CSCSA-42088} \sdd{SF-3963} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Check_Rctb_Qualifier__false_qualifier_is_expected_since_no_brake_qualifier_is_given)
{
   /** \arrange Set up inputs such that false is returned. */
   boolean_T res;
   uint8_t approach_side                               = FBK_SIDE_LEFT;
   uint8_t ctb_mode                                    = CTA_MODE_REAR;
   p_cta_cals->k_ctb_time_to_ask_for_final_brake_decel = 0.0f;

   cta_instance.core_output.cta_alert_level[ctb_mode][approach_side]   = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.f_brake_qualifier[ctb_mode][approach_side] = FBK_FALSE;

   /** \action Test the correct returning of the qualifier. */
   res = Cta_Check_Rctb_Qualifier(&cta_instance.core_output, approach_side, ctb_mode);

   /** \assert expect that the qualifier is set to false due to missing brake qualifier. */
   EXPECT_FALSE(res);
}

/**
 * Check whether the variant position is correctly returned based on the gear status.
 * \uts{CSCSA-42089} \sdd{SF-4052} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Get_Variant_Position__debug_deactivated_status_gear_drive_activated)
{
   /** \arrange Set up gear status accordingly */
   uint8_t position;
   p_cta_cals->k_cta_DEBUG_MODE = FBK_FALSE;
   p_vehicle_data->prndl        = PA_VEH_PRNDL_STATE_DRIVE;
   /** \action Test variant position getter */
   position = Cta_Get_Variant_Position(p_vehicle_data, p_cta_cals);

   /** \assert check whether front position is returned. */
   EXPECT_EQ(position, CTA_MODE_FRONT);
}


/**
 * Check whether the variant position is correctly returned based on the gear status.
 * \uts{CSCSA-42090} \sdd{SF-4052} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Get_Variant_Position__debug_deactivated_status_gear_reverse_activated)
{
   /** \arrange Set up gear status accordingly */
   uint8_t position;
   p_cta_cals->k_cta_DEBUG_MODE = FBK_FALSE;
   p_vehicle_data->prndl        = PA_VEH_PRNDL_STATE_REVERSE;

   /** \action Test variant position getter */
   position = Cta_Get_Variant_Position(p_vehicle_data, p_cta_cals);

   /** \assert check whether rear position is returned. */
   EXPECT_EQ(position, CTA_MODE_REAR);
}


/**
 * Check whether the variant position is correctly returned based on the gear status.
 * \uts{CSCSA-42091} \sdd{SF-4052} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Get_Variant_Position__debug_deactivated_status_neutral_expect_default_output)
{
   /** \arrange Set up gear status accordingly */
   uint8_t position;
   p_cta_cals->k_cta_DEBUG_MODE = FBK_FALSE;
   p_vehicle_data->prndl        = PA_VEH_PRNDL_STATE_NEUTRAL;

   /** \action Test variant position getter */
   position = Cta_Get_Variant_Position(p_vehicle_data, p_cta_cals);

   /** \assert check whether default value is returned. */
   EXPECT_EQ(position, CTA_NUM_MODES);
}


/**
 * Check whether the variant position is correctly returned based on the gear status.
 * \uts{CSCSA-42092} \sdd{SF-4052} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Get_Variant_Position__debug_activated_activate_fcta)
{
   /** \arrange Set up gear status accordingly */
   uint8_t position;
   p_cta_cals->k_cta_DEBUG_MODE                   = FBK_TRUE;
   p_cta_cals->k_cta_enable_modes[CTA_MODE_REAR]  = FBK_FALSE;
   p_cta_cals->k_cta_enable_modes[CTA_MODE_FRONT] = FBK_TRUE;
   p_vehicle_data->prndl                          = PA_VEH_PRNDL_STATE_DRIVE;
   /** \action Test variant position getter */
   position = Cta_Get_Variant_Position(p_vehicle_data, p_cta_cals);

   /** \assert check whether front position is returned. */
   EXPECT_EQ(position, CTA_MODE_FRONT);
}

/**
 * Check whether the variant position is correctly returned based on the gear status.
 * \uts{CSCSA-42093} \sdd{SF-4052} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Get_Variant_Position__debug_activated_activate_rcta)
{
   /** \arrange Set up gear status accordingly */
   uint8_t position;
   p_cta_cals->k_cta_DEBUG_MODE                   = FBK_TRUE;
   p_cta_cals->k_cta_enable_modes[CTA_MODE_REAR]  = FBK_TRUE;
   p_cta_cals->k_cta_enable_modes[CTA_MODE_FRONT] = FBK_FALSE;
   p_vehicle_data->prndl                          = PA_VEH_PRNDL_STATE_REVERSE;
   /** \action Test variant position getter */
   position = Cta_Get_Variant_Position(p_vehicle_data, p_cta_cals);

   /** \assert check whether rear position is returned. */
   EXPECT_EQ(position, CTA_MODE_REAR);
}


/**
 * Check whether the variant position is correctly returned based on the gear status.
 * \uts{CSCSA-42094} \sdd{SF-4052} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Get_Variant_Position__debug_activated_status_neutral_expect_default_output)
{
   /** \arrange Set up gear status accordingly */
   uint8_t position;
   p_cta_cals->k_cta_DEBUG_MODE                   = FBK_TRUE;
   p_cta_cals->k_cta_enable_modes[CTA_MODE_REAR]  = FBK_FALSE;
   p_cta_cals->k_cta_enable_modes[CTA_MODE_FRONT] = FBK_FALSE;
   p_vehicle_data->prndl                          = PA_VEH_PRNDL_STATE_NEUTRAL;
   /** \action Test variant position getter */
   position = Cta_Get_Variant_Position(p_vehicle_data, p_cta_cals);

   /** \assert check whether default value is returned. */
   EXPECT_EQ(position, CTA_NUM_MODES);
}


/**
 * Check whether the warning output mapping is done correctly for acute warning RCTA.
 * \uts{CSCSA-42095} \sdd{SF-4030} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Warning_Outputs__mapping_rcta_acute_warning_left_side)
{
   /** \arrange Set up acute warning for RCTA. */
   Bmw_Ctb_Alert_Side_T side                                            = BMW_CTB_ALERT_SIDE_LEFT;
   cta_output.bmw_ctb_output_algo_state.warn_control_output_state[side] = BMW_CTB_WARN_STATE_ACUTE_WARNING_REAR;

   /** \action Test function for setting of outputs. */
   Cta_Set_Warning_Outputs(&cta_output.bmw_ctb_output_algo_state, side);

   /** \assert check whether correct mapping is applied for BMW_CTB_WARN_STATE_ACUTE_WARNING_REAR. */
   EXPECT_EQ(cta_output.bmw_ctb_output_algo_state.display_warning_graphically[BMW_CTB_ALERT_POSITION_REAR_LEFT], BMW_CTB_WARNING);
   EXPECT_EQ(cta_output.bmw_ctb_output_algo_state.warning_acoustics[BMW_CTB_ALERT_POSITION_REAR_LEFT], BMW_CTB_WARNING);
   EXPECT_EQ(cta_output.bmw_ctb_output_algo_state.ctb_acute_warning[BMW_CTB_ALERT_POSITION_REAR_LEFT], BMW_CTB_WARNING);
}

/**
 * Check whether the warning output mapping is done correctly for acute warning RCTA. Here default values are expected
 * \uts{CSCSA-85518} \sdd{SF-4030} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Warning_Outputs__default_invalid_state)
{
   /** \arrange Set up acute warning for RCTA. */
   Bmw_Ctb_Alert_Side_T side                                            = BMW_CTB_ALERT_SIDE_LEFT;
   cta_output.bmw_ctb_output_algo_state.warn_control_output_state[side] = (Bmw_Ctb_Warn_Control_Output_State_T) 5u;

   /** \action Test function for setting of outputs. */
   Cta_Set_Warning_Outputs(&cta_output.bmw_ctb_output_algo_state, side);

   /** \assert check whether correct mapping is applied for BMW_CTB_WARN_STATE_ACUTE_WARNING_REAR. */
   EXPECT_EQ(cta_output.bmw_ctb_output_algo_state.display_warning_graphically[BMW_CTB_ALERT_POSITION_REAR_LEFT], BMW_CTB_NO_WARNING);
   EXPECT_EQ(cta_output.bmw_ctb_output_algo_state.warning_acoustics[BMW_CTB_ALERT_POSITION_REAR_LEFT], BMW_CTB_NO_WARNING);
   EXPECT_EQ(cta_output.bmw_ctb_output_algo_state.ctb_acute_warning[BMW_CTB_ALERT_POSITION_REAR_LEFT], BMW_CTB_NO_WARNING);
}

/**
 * Check whether the warning output mapping is done correctly for acute warning FCTA.
 * \uts{CSCSA-42096} \sdd{SF-4030} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Warning_Outputs__mapping_fcta_acute_warning_right_side)
{
   /** \arrange Set up acute warning for FCTA. */
   Bmw_Ctb_Alert_Side_T side                                            = BMW_CTB_ALERT_SIDE_RIGHT;
   cta_output.bmw_ctb_output_algo_state.warn_control_output_state[side] = BMW_CTB_WARN_STATE_ACUTE_WARNING_FRONT;

   /** \action Test function for setting of outputs. */
   Cta_Set_Warning_Outputs(&cta_output.bmw_ctb_output_algo_state, side);

   /** \assert check whether correct mapping is applied for BMW_CTB_WARN_STATE_ACUTE_WARNING_FRONT. */
   EXPECT_EQ(cta_output.bmw_ctb_output_algo_state.display_warning_graphically[BMW_CTB_ALERT_POSITION_FRONT_RIGHT], BMW_CTB_WARNING);
   EXPECT_EQ(cta_output.bmw_ctb_output_algo_state.warning_acoustics[BMW_CTB_ALERT_POSITION_FRONT_RIGHT], BMW_CTB_WARNING);
   EXPECT_EQ(cta_output.bmw_ctb_output_algo_state.ctb_acute_warning[BMW_CTB_ALERT_POSITION_FRONT_RIGHT], BMW_CTB_WARNING);
}

/**
 * Check whether the warning output mapping is done correctly for acute warning RCTA.
 * \uts{CSCSA-70351} \sdd{SF-4030} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Warning_Outputs__mapping_rcta_info_warning_left_side)
{
   /** \arrange Set up acute warning for FCTA. */
   Bmw_Ctb_Alert_Side_T side                                            = BMW_CTB_ALERT_SIDE_LEFT;
   cta_output.bmw_ctb_output_algo_state.warn_control_output_state[side] = BMW_CTB_WARN_STATE_INFO_WARNING_FRONT;

   /** \action Test function for setting of outputs. */
   Cta_Set_Warning_Outputs(&cta_output.bmw_ctb_output_algo_state, side);

   /** \assert check whether correct mapping is applied for BMW_CTB_WARN_STATE_ACUTE_WARNING_REAR. */
   EXPECT_EQ(cta_output.bmw_ctb_output_algo_state.display_warning_graphically[BMW_CTB_ALERT_POSITION_FRONT_LEFT], BMW_CTB_WARNING);
   EXPECT_EQ(cta_output.bmw_ctb_output_algo_state.ctb_warning[BMW_CTB_ALERT_POSITION_FRONT_LEFT], BMW_CTB_WARNING);
}

/**
 * Check whether the warning output mapping is done correctly for acute warning FCTA.
 * \uts{CSCSA-70352} \sdd{SF-4030} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Warning_Outputs__mapping_fcta_info_warning_right_side)
{
   /** \arrange Set up acute warning for RCTA. */
   Bmw_Ctb_Alert_Side_T side                                            = BMW_CTB_ALERT_SIDE_RIGHT;
   cta_output.bmw_ctb_output_algo_state.warn_control_output_state[side] = BMW_CTB_WARN_STATE_INFO_WARNING_REAR;

   /** \action Test function for setting of outputs. */
   Cta_Set_Warning_Outputs(&cta_output.bmw_ctb_output_algo_state, side);

   /** \assert check whether correct mapping is applied for BMW_CTB_WARN_STATE_ACUTE_WARNING_FRONT. */
   EXPECT_EQ(cta_output.bmw_ctb_output_algo_state.display_warning_graphically[BMW_CTB_ALERT_POSITION_REAR_RIGHT], BMW_CTB_WARNING);
   EXPECT_EQ(cta_output.bmw_ctb_output_algo_state.ctb_warning[BMW_CTB_ALERT_POSITION_REAR_RIGHT], BMW_CTB_WARNING);
}

/**
 * Check whether the variant position is correctly returned based on the gear status.
 * \uts{CSCSA-70353} \sdd{SF-4052} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Get_Variant_Position__debug_activated_status_neutral_default_output)
{
   /** \arrange Set up gear status accordingly */
   uint8_t position;
   p_cta_cals->k_cta_DEBUG_MODE                   = FBK_TRUE;
   p_cta_cals->k_cta_enable_modes[CTA_MODE_REAR]  = FBK_TRUE;
   p_cta_cals->k_cta_enable_modes[CTA_MODE_FRONT] = FBK_TRUE;
   p_vehicle_data->prndl                          = PA_VEH_PRNDL_STATE_NEUTRAL;
   /** \action Test variant position getter */
   position = Cta_Get_Variant_Position(p_vehicle_data, p_cta_cals);

   /** \assert check whether rear position is returned. */
   EXPECT_EQ(position, CTA_NUM_MODES);
}

/**
 * Check whether the outputs are correctly returned when the CTB state is not in ACTIVE.
 * \uts{CSCSA-70354} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Outputs_Not_Active_State__not_active_state_output)
{
   /** \arrange Current State is UNAVAILABLE */
   output_signals.qualifier_function_ctb = CTB_STATE_NOT_AVAILABLE;

   /** \action Test function for setting of outputs. */
   Cta_Set_Outputs_Not_Active_State(&output_signals /**< BMW Bus Signal Output */);

   /** \assert check whether not available warnings are returned.Expecting No alerts */
   EXPECT_EQ(output_signals.warning_acoustics, CTA_BMW_BIT_POSITION_0);
   EXPECT_EQ(output_signals.target_longitudinal_acceleration, 0.0f);
}

/**
 * Check whether the graphical warning is correctly returned based on FCTA Active State (front right warning).
 * \uts{CSCSA-70355} \sdd{SF-4034} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Get_Alert_Position_Encoding__Active_state_fr_graphical_warning_output)
{
   /** \arrange Set Current State to FCTA ACTIVE.Also Algo Warn State is set to WARNING for FR */
   algo_warn_states.display_warning_graphically[BMW_CTB_ALERT_POSITION_FRONT_RIGHT] = BMW_CTB_WARNING;
   *Ctb_Current_State                                                               = CTB_STATE_FCTA_ACTIVE;

   /** \action Test function for setting bitfields. */
   uint8_t warning = Cta_Get_Alert_Position_Encoding(algo_warn_states.display_warning_graphically, *Ctb_Current_State);

   /** \assert check whether alert is returned..Expecting Alert on Bitfield 1 */
   EXPECT_EQ(warning, CTA_BMW_BIT_POSITION_1);
}
/**
 * Check whether the graphical warning is correctly returned based on FCTA Active State.(front left warning)
 * \uts{CSCSA-70356} \sdd{SF-4034} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Get_Alert_Position_Encoding__fcta_active_state_fl_graphical_warning_output)
{
   /** \arrange Set Current State to FCTA ACTIVE.Also Algo Warn State is set to WARNING for FL */
   algo_warn_states.display_warning_graphically[BMW_CTB_ALERT_POSITION_FRONT_LEFT] = BMW_CTB_WARNING;
   *Ctb_Current_State                                                              = CTB_STATE_FCTA_ACTIVE;

   /** \action Test function for setting bitfields. */
   uint8_t warning = Cta_Get_Alert_Position_Encoding(algo_warn_states.display_warning_graphically, *Ctb_Current_State);

   /** \assert check whether alert is returned..Expecting Alert on Bitfield 2 */
   EXPECT_EQ(warning, CTA_BMW_BIT_POSITION_2);
}
/**
 * Check whether the graphical warning is correctly returned based on FCTA Active State.(rear right warning)
 * \uts{CSCSA-70357} \sdd{SF-4034} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Get_Alert_Position_Encoding__fcta_active_state_rr_graphical_warning_output)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to WARNING for RR */
   algo_warn_states.display_warning_graphically[BMW_CTB_ALERT_POSITION_REAR_RIGHT] = BMW_CTB_WARNING;
   *Ctb_Current_State                                                              = CTB_STATE_FCTA_ACTIVE;

   /** \action Test function for setting bitfields. */
   uint8_t warning = Cta_Get_Alert_Position_Encoding(algo_warn_states.display_warning_graphically, *Ctb_Current_State);

   /** \assert check whether alert is suppressed..Expecting no alert. */
   EXPECT_EQ(warning, CTA_BMW_BIT_POSITION_0);
}
/**
 * Check whether the graphical warning is correctly returned based on RCTA Active State.(rear right warning)
 * \uts{CSCSA-70358} \sdd{SF-4034} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Get_Alert_Position_Encoding__rcta_active_state_rr_graphical_warning_output)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to WARNING for RR */
   algo_warn_states.display_warning_graphically[BMW_CTB_ALERT_POSITION_REAR_RIGHT] = BMW_CTB_WARNING;
   *Ctb_Current_State                                                              = CTB_STATE_RCTA_ACTIVE;

   /** \action Test function for setting bitfields. */
   uint8_t warning = Cta_Get_Alert_Position_Encoding(algo_warn_states.display_warning_graphically, *Ctb_Current_State);

   /** \assert check whether alert is returned.Expecting Alert on Bitfield 3 */
   EXPECT_EQ(warning, CTA_BMW_BIT_POSITION_3);
}

/**
 * Check whether the graphical warning is correctly returned based on RCTA Active State.(rear left warning)
 * \uts{CSCSA-70359} \sdd{SF-4034} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Get_Alert_Position_Encoding__rcta_active_state_rl_graphical_warning_output)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to WARNING for RL */
   algo_warn_states.display_warning_graphically[BMW_CTB_ALERT_POSITION_REAR_LEFT] = BMW_CTB_WARNING;
   *Ctb_Current_State                                                             = CTB_STATE_RCTA_ACTIVE;

   /** \action Test function for setting bitfields. */
   uint8_t warning = Cta_Get_Alert_Position_Encoding(algo_warn_states.display_warning_graphically, *Ctb_Current_State);

   /** \assert check whether alert is returned..Expecting Alert on Bitfield 4 */
   EXPECT_EQ(warning, CTA_BMW_BIT_POSITION_4);
}

/**
 * Check whether the graphical warning is correctly returned based on RCTA Active State.(front right warning)
 * \uts{CSCSA-70360} \sdd{SF-4034} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Get_Alert_Position_Encoding__rcta_active_state_fl_graphical_warning_output)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to WARNING for RL */
   algo_warn_states.display_warning_graphically[BMW_CTB_ALERT_POSITION_FRONT_RIGHT] = BMW_CTB_WARNING;
   *Ctb_Current_State                                                               = CTB_STATE_RCTA_ACTIVE;

   /** \action Test function for setting bitfields. */
   uint8_t warning = Cta_Get_Alert_Position_Encoding(algo_warn_states.display_warning_graphically, *Ctb_Current_State);

   /** \assert check whether alert is suppressed..Expecting No Alert */
   EXPECT_EQ(warning, CTA_BMW_BIT_POSITION_0);
}

/**
 * Check whether the acute warning is correctly returned based on FCTA Active State.(front right warning)
 * \uts{CSCSA-70361} \sdd{SF-4034} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Get_Alert_Position_Encoding__rcta_active_state_fr_acute_warning_output)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to acute WARNING for FR */
   algo_warn_states.ctb_acute_warning[BMW_CTB_ALERT_POSITION_FRONT_RIGHT] = BMW_CTB_WARNING;
   *Ctb_Current_State                                                     = CTB_STATE_FCTA_ACTIVE;

   /** \action Test function for setting bitfields. */
   uint8_t warning = Cta_Get_Alert_Position_Encoding(algo_warn_states.ctb_acute_warning, *Ctb_Current_State);

   /** \assert check whether alert is set. .Expecting Alert on Bitfield 1 */
   EXPECT_EQ(warning, CTA_BMW_BIT_POSITION_1);
}

/**
 * Check whether the acoustic warning is correctly returned based on RCTA Active State.(rear right warning)
 * \uts{CSCSA-70362} \sdd{SF-4034} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Get_Alert_Position_Encoding__rcta_active_state_rr_acoustic_warning_output)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to acoustic WARNING for RR */
   algo_warn_states.warning_acoustics[BMW_CTB_ALERT_POSITION_REAR_RIGHT] = BMW_CTB_WARNING;
   *Ctb_Current_State                                                    = CTB_STATE_RCTA_ACTIVE;

   /** \action Test function for setting bitfields. */
   uint8_t warning = Cta_Get_Alert_Position_Encoding(algo_warn_states.warning_acoustics, *Ctb_Current_State);

   /** \assert check whether alert is set.Expecting Alert on Bitfield 3 */
   EXPECT_EQ(warning, CTA_BMW_BIT_POSITION_3);
}

/**
 * Check whether the ctb warning is correctly returned based on RCTA Active State.(rear right warning)
 * \uts{CSCSA-70363} \sdd{SF-4034} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Get_Alert_Position_Encoding__rcta_active_state_rl_ctb_warning_output)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to acoustic WARNING for RR */
   algo_warn_states.ctb_warning[BMW_CTB_ALERT_POSITION_REAR_RIGHT] = BMW_CTB_WARNING;
   *Ctb_Current_State                                              = CTB_STATE_RCTA_ACTIVE;

   /** \action Test function for setting bitfields. */
   uint8_t warning = Cta_Get_Alert_Position_Encoding(algo_warn_states.ctb_warning, *Ctb_Current_State);

   /** \assert check whether alert is set.Expecting Alert on Bitfield 3 */
   EXPECT_EQ(warning, CTA_BMW_BIT_POSITION_3);
}

/**
 * Check whether the acoustic warning is correctly returned based on RCTA Active State.(rear right warning)
 * \uts{CSCSA-70364} \sdd{SF-4034} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Get_Alert_Position_Encoding__rcta_active_state_rr_no_warning_output)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to acoustic WARNING for RR */
   algo_warn_states.warning_acoustics[BMW_CTB_ALERT_POSITION_REAR_RIGHT] = BMW_CTB_NO_WARNING;
   *Ctb_Current_State                                                    = CTB_STATE_RCTA_ACTIVE;

   /** \action Test function for setting bitfields. */
   uint8_t warning = Cta_Get_Alert_Position_Encoding(algo_warn_states.warning_acoustics, *Ctb_Current_State);

   /** \assert check whether alert is set.Expecting Alert on Bitfield 3 */
   EXPECT_EQ(warning, CTA_BMW_BIT_POSITION_0);
}

/**
 * Check whether the extmirrot warning is correctly returned based on RCTA Active State.(rear right warning)
 * \uts{CSCSA-70365} \sdd{SF-4028} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__rcta_active_state_rr_warning_output)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to acoustic WARNING for RR */
   algo_warn_states.request_extmirror_warning[BMW_CTB_ALERT_SIDE_LEFT] = BMW_CTB_REQUEST_MIRROR_DISPLAY_SEGMENT_ON_FLASHING_LEVEL_1;
   algo_warn_states.request_extmirror_warning[BMW_CTB_ALERT_SIDE_RIGHT] = BMW_CTB_REQUEST_MIRROR_DIS_PL_AY_SEGMENT_OFF;
   *Ctb_Current_State                                                   = CTB_STATE_RCTA_ACTIVE;

   // Bmw_Ctb_Output_Bus_Signals_T
   /** \action Test function for setting bitfields. */
   Cta_Set_Output_Bus_Signals(&output_signals, &algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(output_signals.request_extmirror_warning_left, BMW_CTB_REQUEST_MIRROR_DISPLAY_SEGMENT_ON_FLASHING_LEVEL_1);
   EXPECT_EQ(output_signals.request_extmirror_warning_right, BMW_CTB_REQUEST_MIRROR_DIS_PL_AY_SEGMENT_OFF);
}

/**
 * Check whether the extmirrot warning is correctly returned based on FCTA Active State.(No warning)
 * \uts{CSCSA-70366} \sdd{SF-4028} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__fcta_active_state_rr_warning_output)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to acoustic WARNING for RR */
   algo_warn_states.request_extmirror_warning[BMW_CTB_ALERT_SIDE_LEFT] = BMW_CTB_REQUEST_MIRROR_DISPLAY_SEGMENT_ON_FLASHING_LEVEL_1;
   algo_warn_states.request_extmirror_warning[BMW_CTB_ALERT_SIDE_RIGHT] = BMW_CTB_REQUEST_MIRROR_DIS_PL_AY_SEGMENT_OFF;
   *Ctb_Current_State                                                   = CTB_STATE_FCTA_ACTIVE;
   // Bmw_Ctb_Output_Bus_Signals_T
   /** \action Test function for setting bitfields. */
   Cta_Set_Output_Bus_Signals(&output_signals, &algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(output_signals.request_extmirror_warning_left, BMW_CTB_REQUEST_MIRROR_DIS_PL_AY_SEGMENT_OFF);
   EXPECT_EQ(output_signals.request_extmirror_warning_right, BMW_CTB_REQUEST_MIRROR_DIS_PL_AY_SEGMENT_OFF);
}

/**
 * Check whether the braking is correctly returned based on FCTA Active State.(No warning)
 * \uts{CSCSA-70367} \sdd{SF-4028} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__fcta_brake_state_output)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to acoustic WARNING for RR */
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant = BMW_CTB_VARIANT_CTB_REAR_AND_FRONT;
   *Ctb_Current_State                                = CTB_STATE_FCTA_ACTIVE;
   algo_warn_states.target_longitudinal_acceleration = -2.0f;
   algo_warn_states.status_brake_requirement         = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
   // Bmw_Ctb_Output_Bus_Signals_T
   /** \action Test function for setting bitfields. */
   Cta_Set_Output_Bus_Signals(&output_signals, &algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(output_signals.target_longitudinal_acceleration, -2.0f);
   EXPECT_EQ(output_signals.ctb_braking, BMW_CTB_BRAKING_AT_FRONT);
}

/**
 * Check whether the braking is not returned based on FCTA Active State.(No warning) when variant is rear braking
 * \uts{CSCSA-70368} \sdd{SF-4028} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__fcta_brake_state_output_variant_rear)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to acoustic WARNING for RR */
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant = BMW_CTB_VARIANT_CTB_REAR;
   *Ctb_Current_State                                = CTB_STATE_FCTA_ACTIVE;
   algo_warn_states.target_longitudinal_acceleration = -2.0f;
   algo_warn_states.status_brake_requirement         = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
   /** \action Test function for setting bitfields. */
   Cta_Set_Output_Bus_Signals(&output_signals, &algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(output_signals.target_longitudinal_acceleration, 0.0f);
   EXPECT_EQ(output_signals.ctb_braking, BMW_CTB_NO_BRAKING);
}

/**
 * Check whether the braking is returned based on FCTA Active State.(No warning) when variant is rear braking
 * \uts{CSCSA-70369} \sdd{SF-4028} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__fcta_brake_state_output_variant_braking_state)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to acoustic WARNING for RR */
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant                       = BMW_CTB_VARIANT_CTB_REAR_AND_FRONT;
   *Ctb_Current_State                                                      = CTB_STATE_RCTA_ACTIVE;
   algo_warn_states.target_longitudinal_acceleration                       = -2.0f;
   algo_warn_states.status_brake_requirement                               = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
   cta_input.bmw_ctb_input_signals.status_acceleration_long_prioritization = CTA_BMW_BIT_POSITION_0;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check      = FBK_TRUE;
   /** \action Test function for setting bitfields. */
   Cta_Set_Output_Bus_Signals(&output_signals, &algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(output_signals.target_longitudinal_acceleration, 0.0f);
   EXPECT_EQ(output_signals.ctb_braking, BMW_CTB_NO_BRAKING);
}
/**
 * Check whether the braking is returned based on FCTA Active State.(No warning) when banner time is less
 * \uts{CSCSA-70370} \sdd{SF-4028} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__fcta_brake_state_output_variant_banner_time)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to acoustic WARNING for RR */
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant                       = BMW_CTB_VARIANT_CTB_REAR_AND_FRONT;
   *Ctb_Current_State                                                      = CTB_STATE_RCTA_ACTIVE;
   algo_warn_states.target_longitudinal_acceleration                       = -2.0f;
   algo_warn_states.status_brake_requirement                               = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
   cta_input.bmw_ctb_input_signals.status_acceleration_long_prioritization = CTA_BMW_BIT_POSITION_1;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check      = FBK_TRUE;
   cta_instance.customer_calibration.k_bmw_sp25_banner_time                = -0.5f;
   /** \action Test function for setting bitfields. */
   Cta_Set_Output_Bus_Signals(&output_signals, &algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(output_signals.target_longitudinal_acceleration, 0.0f);
   EXPECT_EQ(output_signals.ctb_braking, BMW_CTB_NO_BRAKING);
}

/**
 * Check whether the braking is not returned based on FCTA Active State.(No warning) when brake requirement is No braking
 * \uts{CSCSA-70371} \sdd{SF-4028} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__fcta_brake_state_output_brake_requirement)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to acoustic WARNING for RR */
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant = BMW_CTB_VARIANT_CTB_REAR_AND_FRONT;
   *Ctb_Current_State                                = CTB_STATE_FCTA_ACTIVE;
   algo_warn_states.target_longitudinal_acceleration = -2.0f;
   algo_warn_states.status_brake_requirement         = BMW_CTB_STATUS_BRAKE_REQ_NO_BRAKING;
   /** \action Test function for setting bitfields. */
   Cta_Set_Output_Bus_Signals(&output_signals, &algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(output_signals.target_longitudinal_acceleration, 0.0f);
   EXPECT_EQ(output_signals.ctb_braking, BMW_CTB_NO_BRAKING);
}

/**
 * Check whether the braking is returned based on RCTA Active State.(No warning) when variant is rear braking
 * \uts{CSCSA-70372} \sdd{SF-4028} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__rcta_brake_state_output_variant_rear)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to acoustic WARNING for RR */
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant = BMW_CTB_VARIANT_CTB_REAR;
   *Ctb_Current_State                                = CTB_STATE_RCTA_ACTIVE;
   algo_warn_states.target_longitudinal_acceleration = -2.0f;
   algo_warn_states.status_brake_requirement         = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
   /** \action Test function for setting bitfields. */
   Cta_Set_Output_Bus_Signals(&output_signals, &algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(output_signals.target_longitudinal_acceleration, -2.0f);
   EXPECT_EQ(output_signals.ctb_braking, BMW_CTB_BRAKING_AT_REAR);
}

/**
 * Check whether the braking is returned based on RCTA Active State.(No warning) when variant is rear braking
 * \uts{CSCSA-70373} \sdd{SF-4028} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__rcta_brake_state_output_variant_no_brake)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to acoustic WARNING for RR */
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant = BMW_CTB_VARIANT_NO_CTB;
   *Ctb_Current_State                                = CTB_STATE_RCTA_ACTIVE;
   algo_warn_states.target_longitudinal_acceleration = -2.0f;
   algo_warn_states.status_brake_requirement         = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
   /** \action Test function for setting bitfields. */
   Cta_Set_Output_Bus_Signals(&output_signals, &algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(output_signals.target_longitudinal_acceleration, 0.0f);
   EXPECT_EQ(output_signals.ctb_braking, BMW_CTB_NO_BRAKING);
}

/**
 * Check whether the braking is returned based on RCTA Active State.(No warning) when braking state is false
 * \uts{CSCSA-70374} \sdd{SF-4028} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__rcta_brake_state_output_variant_braking_state)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to acoustic WARNING for RR */
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant                       = BMW_CTB_VARIANT_CTB_REAR;
   *Ctb_Current_State                                                      = CTB_STATE_RCTA_ACTIVE;
   algo_warn_states.target_longitudinal_acceleration                       = -2.0f;
   algo_warn_states.status_brake_requirement                               = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
   cta_input.bmw_ctb_input_signals.status_acceleration_long_prioritization = CTA_BMW_BIT_POSITION_0;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check      = FBK_TRUE;
   /** \action Test function for setting bitfields. */
   Cta_Set_Output_Bus_Signals(&output_signals, &algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(output_signals.target_longitudinal_acceleration, 0.0f);
   EXPECT_EQ(output_signals.ctb_braking, BMW_CTB_NO_BRAKING);
}

/**
 * Check whether the braking is returned based on RCTA Active State.(No warning) when banner time is less
 * \uts{CSCSA-70375} \sdd{SF-4028} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__rcta_brake_state_output_variant_banner_time)
{
   /** \arrange Set Current State to RCTA ACTIVE.Also Algo Warn State is set to acoustic WARNING for RR */
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant                       = BMW_CTB_VARIANT_CTB_REAR;
   *Ctb_Current_State                                                      = CTB_STATE_RCTA_ACTIVE;
   algo_warn_states.target_longitudinal_acceleration                       = -2.0f;
   algo_warn_states.status_brake_requirement                               = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
   cta_input.bmw_ctb_input_signals.status_acceleration_long_prioritization = CTA_BMW_BIT_POSITION_1;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check      = FBK_TRUE;
   cta_instance.customer_calibration.k_bmw_sp25_banner_time                = -0.5f;
   /** \action Test function for setting bitfields. */
   Cta_Set_Output_Bus_Signals(&output_signals, &algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(output_signals.target_longitudinal_acceleration, 0.0f);
   EXPECT_EQ(output_signals.ctb_braking, BMW_CTB_NO_BRAKING);
}

/**
 * Check whether the RCTA_ACTIVE is converted to ACTIVE state
 * \uts{CSCSA-70378} \sdd{SF-4028} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__rcta_active_to_active_state)
{
   /** \arrange Set Current State to RCTA DEGRADED. */
   *Ctb_Current_State = CTB_STATE_RCTA_ACTIVE;

   /** \action Test function for setting bitfields. */
   Cta_Set_Output_Bus_Signals(&output_signals, &algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(output_signals.qualifier_function_ctb, CTB_STATE_ACTIVE);
}

/**
 * Check whether the FCTA_ACTIVE is converted to ACTIVE state
 * \uts{CSCSA-70379} \sdd{SF-4028} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__fcta_active_to_active_state)
{
   /** \arrange Set Current State to FCTA DEGRADED. */
   *Ctb_Current_State = CTB_STATE_FCTA_ACTIVE;

   /** \action Test function for setting bitfields. */
   Cta_Set_Output_Bus_Signals(&output_signals, &algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(output_signals.qualifier_function_ctb, CTB_STATE_ACTIVE);
}

/**
 * Check whether the FCTA ACTIVE state is converted to ACTIVE BRAKING state when braking is active
 * \uts{CSCSA-70380} \sdd{SF-4028} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__fcta_active_to_active_braking_state)
{
   /** \arrange Set Current State to FCTA DEGRADED. */
   *Ctb_Current_State                                = CTB_STATE_FCTA_ACTIVE;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant = BMW_CTB_VARIANT_CTB_REAR_AND_FRONT;
   algo_warn_states.status_brake_requirement         = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
   /** \action Test function for setting bitfields. */
   Cta_Set_Output_Bus_Signals(&output_signals, &algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(output_signals.qualifier_function_ctb, CTB_STATE_ACTIVE_BRAKING);
}

/**
 * Check whether RCTA ACTIVE state is converted to ACTIVE BRAKING state when braking is active
 * \uts{CSCSA-70381} \sdd{SF-4028} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__rcta_active_to_active_braking_state)
{
   /** \arrange Set Current State to FCTA DEGRADED. */
   *Ctb_Current_State                                = CTB_STATE_RCTA_ACTIVE;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant = BMW_CTB_VARIANT_CTB_REAR;
   algo_warn_states.status_brake_requirement         = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
   /** \action Test function for setting bitfields. */
   Cta_Set_Output_Bus_Signals(&output_signals, &algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(output_signals.qualifier_function_ctb, CTB_STATE_ACTIVE_BRAKING);
}

/**
 * Check whether Banner Check Sets to TRUE by default
 * \uts{CSCSA-70382} \sdd{SF-4052} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Banner_Check_Flag__default_TRUE)
{
   /** \arrange Set Current State to FCTA DEGRADED. */
   boolean_T banner_check_out = FBK_FALSE;
   /** \action Test function for setting bitfields. */
   banner_check_out = Cta_Set_Banner_Check_Flag(&algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(banner_check_out, FBK_TRUE);
}
/**
 * Check whether Banner Check Sets to TRUE when banner check criteria is TRUE
 * \uts{CSCSA-70383} \sdd{SF-4052} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Banner_Check_Flag__banner_check_calib_true)
{
   /** \arrange Set Current State to FCTA DEGRADED. */
   boolean_T banner_check_out                                                    = FBK_FALSE;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check            = FBK_TRUE;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check_time       = -1.0f;
   cta_input.bmw_ctb_input_signals.status_acceleration_long_prioritization       = CTA_FASOCLI_ACCELERATION_PRIORITIZED;
   cta_input.bmw_ctb_input_signals.status_arbitration_longitudinal_low_integrity = CTA_ACCELERATION_PRIORITIZED;
   algo_warn_states.status_brake_requirement                                     = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
   /** \action Test function for setting bitfields. */
   banner_check_out = Cta_Set_Banner_Check_Flag(&algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(banner_check_out, FBK_TRUE);
}

/**
 * Check whether Banner Check Sets to TRUE when banner check criteria is TRUE
 * \uts{CSCSA-70384} \sdd{SF-4052} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Banner_Check_Flag__banner_check_calib_true_check_time)
{
   /** \arrange Set Current State to FCTA DEGRADED. */
   boolean_T banner_check_out                                              = FBK_FALSE;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check      = FBK_TRUE;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check_time = 10.0f;
   cta_input.bmw_ctb_input_signals.status_acceleration_long_prioritization = CTA_BMW_BIT_POSITION_1;
   algo_warn_states.status_brake_requirement                               = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
   /** \action Test function for setting bitfields. */
   banner_check_out = Cta_Set_Banner_Check_Flag(&algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(banner_check_out, FBK_FALSE);
}

/**
 * Check whether Banner Check Sets to TRUE when banner check criteria is TRUE but time is not elapsed
 * \uts{CSCSA-70385} \sdd{SF-4052} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Banner_Check_Flag__banner_check_calib_false)
{
   /** \arrange Set Current State to FCTA DEGRADED. */
   boolean_T banner_check_out                                                    = FBK_FALSE;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check            = FBK_TRUE;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check_time       = 0.1f;
   cta_input.bmw_ctb_input_signals.status_arbitration_longitudinal_low_integrity = CTA_BMW_BIT_POSITION_1;
   algo_warn_states.status_brake_requirement                                     = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
   /** \action Test function for setting bitfields. */
   banner_check_out = Cta_Set_Banner_Check_Flag(&algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(banner_check_out, FBK_FALSE);
}

/**
 * Check whether Banner Check Sets to TRUE when banner check criteria is FALSE when the inputs are not correct
 * \uts{CSCSA-70386} \sdd{SF-4052} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Banner_Check_Flag__banner_check_input_false)
{
   /** \arrange Set Current State to FCTA DEGRADED. */
   boolean_T banner_check_out                                                    = FBK_FALSE;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check            = FBK_TRUE;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check_time       = 0.0f;
   cta_input.bmw_ctb_input_signals.status_acceleration_long_prioritization       = CTA_BMW_BIT_POSITION_1;
   cta_input.bmw_ctb_input_signals.status_arbitration_longitudinal_low_integrity = CTA_BMW_BIT_POSITION_2;
   algo_warn_states.status_brake_requirement                                     = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
   /** \action Test function for setting bitfields. */
   banner_check_out = Cta_Set_Banner_Check_Flag(&algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(banner_check_out, FBK_FALSE);
}

/**
 * Check whether Banner Check Sets to TRUE when banner check criteria is TRUE
 * \uts{CSCSA-70387} \sdd{SF-4052} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Banner_Check_Flag__banner_check_Prioritization)
{
   /** \arrange Set Current State to FCTA DEGRADED. */
   boolean_T banner_check_out                                              = FBK_FALSE;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check      = FBK_TRUE;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check_time = 0.0f;
   cta_input.bmw_ctb_input_signals.status_acceleration_long_prioritization = CTA_BMW_BIT_POSITION_0;
   algo_warn_states.status_brake_requirement                               = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
   /** \action Test function for setting bitfields. */
   banner_check_out = Cta_Set_Banner_Check_Flag(&algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(banner_check_out, FBK_FALSE);
}

/**
 * Check whether Banner Check Sets to TRUE when banner check criteria is TRUE
 * \uts{CSCSA-70388} \sdd{SF-4052} \testtype{negative}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Banner_Check_Flag__banner_check_brake_requirement)
{
   /** \arrange Set Current State to FCTA DEGRADED. */
   boolean_T banner_check_out                                              = FBK_FALSE;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check      = FBK_TRUE;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check_time = 0.0f;
   cta_input.bmw_ctb_input_signals.status_acceleration_long_prioritization = CTA_BMW_BIT_POSITION_1;
   algo_warn_states.status_brake_requirement                               = BMW_CTB_STATUS_BRAKE_REQ_NO_BRAKING;
   /** \action Test function for setting bitfields. */
   banner_check_out = Cta_Set_Banner_Check_Flag(&algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(banner_check_out, FBK_FALSE);
}

/**
 * Check that the post run is filling the required sinput.
 * \uts{CSCSA-70389} \sdd{SF-3947} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Post_Run__check_that_input_is_set_up)
{
   /** \arrange Set Current State to FCTA DEGRADED. */
   p_cta_cals->k_cta_DEBUG_MODE                   = FBK_TRUE;
   p_cta_cals->k_cta_enable_modes[CTA_MODE_REAR]  = FBK_FALSE;
   p_cta_cals->k_cta_enable_modes[CTA_MODE_FRONT] = FBK_TRUE;

   /** \action Test function for setting bitfields. */
   Cta_Post_Run(&cta_instance, &cta_input, &cta_output);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(cta_output.bmw_ctb_output_algo_state.warn_control_output_state[BMW_CTB_ALERT_SIDE_LEFT], BMW_CTB_WARN_STATE_NO_WARNING);
   EXPECT_EQ(cta_output.bmw_ctb_output_algo_state.warn_control_output_state[BMW_CTB_ALERT_SIDE_RIGHT], BMW_CTB_WARN_STATE_NO_WARNING);
}

/**
 * Check set_braking_and_accelaration_fcta_braking_state
 * \uts{CSCSA-70390} \sdd{SF-4028} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__set_braking_and_accelaration_fcta_braking_status)
{
   /** \arrange Set Current State to RCTA DEGRADED. */
   *Ctb_Current_State                                                 = CTB_STATE_FCTA_ACTIVE;
   algo_warn_states.target_longitudinal_acceleration                  = 2.0f;
   algo_warn_states.status_brake_requirement                          = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant                  = BMW_CTB_VARIANT_CTB_REAR_AND_FRONT;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check = FBK_TRUE;
   cta_instance.customer_calibration.k_bmw_sp25_banner_time           = 2.0f;
   /** \action Test function for setting bitfields. */
   Cta_Set_Output_Bus_Signals(&output_signals, &algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(output_signals.ctb_braking, BMW_CTB_NO_BRAKING);
   EXPECT_EQ(output_signals.target_longitudinal_acceleration, 0.0f);
}

/**
 * Check set_braking_and_accelaration_rcta_braking_banner_time
 * \uts{CSCSA-70391} \sdd{SF-4028} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__set_braking_and_accelaration_rcta_braking_banner_time)
{
   /** \arrange Set Current State to RCTA DEGRADED. */
   *Ctb_Current_State                                                 = CTB_STATE_RCTA_ACTIVE;
   algo_warn_states.target_longitudinal_acceleration                  = 2.0f;
   algo_warn_states.status_brake_requirement                          = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant                  = BMW_CTB_VARIANT_CTB_REAR_AND_FRONT;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check = FBK_FALSE;
   Cta_Banner_Time_Inc_Rcta                                           = 5.0f;
   cta_instance.customer_calibration.k_bmw_sp25_banner_time           = 2.0f;
   /** \action Test function for setting bitfields. */
   Cta_Set_Output_Bus_Signals(&output_signals, &algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(output_signals.ctb_braking, BMW_CTB_NO_BRAKING);
   EXPECT_EQ(output_signals.target_longitudinal_acceleration, 0.0f);
}

/**
 * Check set_braking_and_accelaration_fcta_braking_banner_time
 * \uts{CSCSA-70392} \sdd{SF-4028} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__set_braking_and_accelaration_fcta_braking_banner_time)
{
   /** \arrange Set Current State to RCTA DEGRADED. */
   *Ctb_Current_State                                                 = CTB_STATE_FCTA_ACTIVE;
   algo_warn_states.target_longitudinal_acceleration                  = 2.0f;
   algo_warn_states.status_brake_requirement                          = BMW_CTB_STATUS_BRAKE_REQ_BRAKING;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant                  = BMW_CTB_VARIANT_CTB_REAR_AND_FRONT;
   cta_instance.customer_calibration.k_bmw_sp25_banner_criteria_check = FBK_FALSE;
   Cta_Banner_Time_Inc_Fcta                                           = 5.0f;
   cta_instance.customer_calibration.k_bmw_sp25_banner_time           = 2.0f;
   /** \action Test function for setting bitfields. */
   Cta_Set_Output_Bus_Signals(&output_signals, &algo_warn_states, &cta_input, &cta_instance);

   /** \assert check whether alert is set.for left mirror and not right mirror */
   EXPECT_EQ(output_signals.ctb_braking, BMW_CTB_NO_BRAKING);
   EXPECT_EQ(output_signals.target_longitudinal_acceleration, 0.0f);
}

/**
 * Check that braking output is set correctly, here empty core output is passed, so no braking is expected.
 * \uts{CSCSA-85519} \sdd{SF-4028} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__empty)
{
   /** \arrange Set empty core output. */

   /** \action Test function for setting braking output. */
   Cta_Set_Braking_Outputs(&algo_warn_states, &cta_input, &cta_instance, CTA_MODE_REAR);

   /** \assert check whether brake is not triggered */
   EXPECT_EQ(algo_warn_states.ctb_braking, BMW_CTB_NO_BRAKING);
   EXPECT_EQ(algo_warn_states.status_brake_requirement, BMW_CTB_STATUS_BRAKE_REQ_NO_BRAKING);
   EXPECT_EQ(algo_warn_states.target_longitudinal_acceleration, FBK_ZERO_F);
}

/**
 * Check that braking output is set correctly, here left alert is expected;
 * \uts{CSCSA-85520} \sdd{SF-4028} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__left_warning)
{
   /** \arrange Set core output with left alert. */
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT]   = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.f_brake_qualifier[CTA_MODE_REAR][FBK_SIDE_LEFT] = FBK_TRUE;

   /** \action Test function for setting braking output. */
   Cta_Set_Braking_Outputs(&algo_warn_states, &cta_input, &cta_instance, CTA_MODE_REAR);

   /** \assert check whether brake is triggered */
   EXPECT_EQ(algo_warn_states.status_brake_requirement, BMW_CTB_STATUS_BRAKE_REQ_BRAKING);
}

/**
 * Check that braking output is set correctly, here right alert is expected;
 * \uts{CSCSA-85521} \sdd{SF-4028} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Bus_Signals__right_warning)
{
   /** \arrange Set core output with right alert. */
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT]   = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.f_brake_qualifier[CTA_MODE_REAR][FBK_SIDE_RIGHT] = FBK_TRUE;

   /** \action Test function for setting braking output. */
   Cta_Set_Braking_Outputs(&algo_warn_states, &cta_input, &cta_instance, CTA_MODE_REAR);

   /** \assert check whether brake is triggered */
   EXPECT_EQ(algo_warn_states.status_brake_requirement, BMW_CTB_STATUS_BRAKE_REQ_BRAKING);
}

/**
 * Check that Unique IDs are set correctly;
 * \uts{}} \sdd{} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Algo_State__rear_mode_unique_ids_passed_correctly)
{
   /** \arrange Set core output with right alert. */
   cta_instance.core_output.cta_unique_id[CTA_MODE_REAR][FBK_SIDE_LEFT]  = 9u;
   cta_instance.core_output.cta_unique_id[CTA_MODE_REAR][FBK_SIDE_RIGHT] = 10u;

   /** \action Test function for setting braking output. */
   Cta_Set_Output_Algo_State(&algo_warn_states, &cta_input, &cta_instance, p_vehicle_data, CTA_MODE_REAR);

   /** \assert check whether brake is triggered */
   EXPECT_EQ(algo_warn_states.unique_id[2], cta_instance.core_output.cta_unique_id[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   EXPECT_EQ(algo_warn_states.unique_id[3], cta_instance.core_output.cta_unique_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
}

/**
 * Check that Unique IDs are set correctly;
 * \uts{}} \sdd{} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output_Algo_State__front_mode_unique_ids_passed_correctly)
{
   /** \arrange Set core output with right alert. */
   cta_instance.core_output.cta_unique_id[CTA_MODE_FRONT][FBK_SIDE_LEFT]  = 9u;
   cta_instance.core_output.cta_unique_id[CTA_MODE_FRONT][FBK_SIDE_RIGHT] = 10u;

   /** \action Test function for setting braking output. */
   Cta_Set_Output_Algo_State(&algo_warn_states, &cta_input, &cta_instance, p_vehicle_data, CTA_MODE_FRONT);

   /** \assert check whether brake is triggered */
   EXPECT_EQ(algo_warn_states.unique_id[0], cta_instance.core_output.cta_unique_id[CTA_MODE_FRONT][FBK_SIDE_LEFT]);
   EXPECT_EQ(algo_warn_states.unique_id[1], cta_instance.core_output.cta_unique_id[CTA_MODE_FRONT][FBK_SIDE_RIGHT]);
}
