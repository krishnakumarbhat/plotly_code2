/**
 * @file cta_state_machine_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SP25 CTA state machine
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */
/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{WI-19906}
 */
#include "cta_state_machine_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
extern "C"
{
#include "cta_core_calibration_t.h"
#include "fbk_macros.h"
}
/**
 * Check whether the f_ctb_flag_ctb_enabled is TRUE as expected
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__ctb_flag_ctb_enabled_pos)
{
   /** \arrange inputs for Enabled Flag: TRUE */
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled = FBK_TRUE;
   p_vehicle_data->host_speed                        = 5.0f;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_ctb_flag_ctb_enabled, FBK_TRUE);
}
/**
 * Check whether the f_ctb_flag_ctb_enabled is FALSE as expected
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__ctb_flag_ctb_enabled_neg)
{
   /** \arrange inputs for Enabled Flag : FALSE */
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled = FBK_FALSE;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_ctb_flag_ctb_enabled, FBK_FALSE);
}
/**
 * Check whether the f_ctb_flag_ctb_activated is TRUE as expected
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__ctb_flag_ctb_activated_pos)
{
   /** \arrange inputs for Enabled Flag : TRUE and the brake configurations ACTIVATED */
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled                        = FBK_TRUE;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake              = BMW_CTB_ACTIVATED;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake_with_braking = BMW_CTB_ACTIVATED;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_ctb_flag_ctb_activated, FBK_TRUE);
}
/**
 * Check whether the f_ctb_flag_ctb_activated is TRUE as expected
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__ctb_flag_ctb_with_braking_activated_pos)
{
   /** \arrange inputs for Enabled Flag : TRUE and the brake configurations ACTIVATED */
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled                        = FBK_TRUE;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake              = BMW_CTB_NOT_CONFIGURABLE;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake_with_braking = BMW_CTB_ACTIVATED;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_ctb_flag_ctb_activated, FBK_TRUE);
}
/**
 * Check whether the f_ctb_flag_ctb_activated is TRUE as expected
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__ctb_flag_ctb_with_braking_rcta_active_false)
{
   /** \arrange inputs for Enabled Flag : TRUE and the brake configurations ACTIVATED */
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled                        = FBK_TRUE;
   ctb_state_machine_flag.f_ctb_flag_ctb_activated                          = FBK_TRUE;
   ctb_state_machine_flag.f_ctb_flag_ctb_enabled                            = FBK_TRUE;
   p_vehicle_data->prndl                                                    = PA_VEH_PRNDL_STATE_REVERSE;
   cta_input.bmw_ctb_input_signals.status_trailer                           = BMW_CTB_TRAILER_AVAILABLE;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake              = BMW_CTB_NOT_CONFIGURABLE;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake_with_braking = BMW_CTB_ACTIVATED;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_ctb_flag_ctb_activated, FBK_TRUE);
}
/**
 * Check whether the f_ctb_flag_ctb_activated is FALSE as expected
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__ctb_flag_ctb_activated_neg)
{
   /** \arrange inputs for Enabled Flag TRUE and the brake configurations: NOT CONFIGURABLE */
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled                        = FBK_TRUE;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake              = BMW_CTB_NOT_CONFIGURABLE;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake_with_braking = BMW_CTB_NOT_CONFIGURABLE;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_ctb_flag_ctb_activated, FBK_FALSE);
}
/**
 * Check whether the f_ctb_test_mode flag is TRUE as expected with front axle dynamometer
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__ctb_test_mode_pos_front_axle)
{
   /** \arrange inputs for Roller Dynamometer setting : front axle dynamometer*/
   cta_input.bmw_ctb_input_signals.status_roller_dynamometer = BMW_CTB_FRONT_AXLE_ON_DYNAMOMETER;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_ctb_test_mode, FBK_TRUE);
}
/**
 * Check whether the f_ctb_test_mode flag is TRUE as expected with rear axle dynamometer
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__ctb_test_mode_pos_rear_axle)
{
   /** \arrange inputs for Roller Dynamometer : rear axle dynamometer  */
   cta_input.bmw_ctb_input_signals.status_roller_dynamometer = BMW_CTB_BACK_AXLE_ON_DYNAMOMETER;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_ctb_test_mode, FBK_TRUE);
}
/**
 * Check whether the f_ctb_test_mode flag is TRUE as expected with two axle dynamometer
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__ctb_test_mode_pos_two_axle)
{
   /** \arrange inputs for Roller Dynamometer : two axle dynamometer */
   cta_input.bmw_ctb_input_signals.status_roller_dynamometer = BMW_CTB_TWO_AXLE_DYNAMOMETER;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_ctb_test_mode, FBK_TRUE);
}
/**
 * Check whether the f_ctb_test_mode flag is TRUE as expected with end of line mode
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__ctb_test_mode_pos_eol_set)
{
   /** \arrange inputs Status End of Line to MODE SET */
   cta_input.bmw_ctb_input_signals.status_end_of_line = END_OF_LINE_MODE_SET;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_ctb_test_mode, FBK_TRUE);
}
/**
 * Check whether the f_ctb_test_mode flag is FALSE as expected
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__ctb_test_mode_false)
{
   /** \arrange input for end of line not set */
   cta_input.bmw_ctb_input_signals.status_end_of_line = END_OF_LINE_MODE_NOT_SET;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_ctb_test_mode, FBK_FALSE);
}
/**
 * Check whether the f_speed_check flag is TRUE as expected
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__speed_check_true)
{
   /** \arrange inputs vehicle speed<threshold set */
   p_vehicle_data->host_speed = 1.39f;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_speed_check, FBK_TRUE);
}
/**
 * Check whether the f_speed_check flag is FALSE as expected
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__speed_check_false)
{
   /** \arrange inputs vehicle speed>threshold set */
   p_vehicle_data->host_speed = 16.67f;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_speed_check, FBK_FALSE);
}
/**
 * Check whether the f_speed_check_hys flag is TRUE as expected
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__speed_check_hys_true)
{
   /** \arrange inputs vehicle speed>threshold set */
   p_vehicle_data->host_speed = 15.31f;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_speed_check_hys, FBK_TRUE);
}
/**
 * Check whether the f_speed_check_hys flag is FALSE as expected
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__speed_check_hys_false)
{
   /** \arrange inputs vehicle speed<threshold set */
   p_vehicle_data->host_speed = 1.67f;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_speed_check_hys, FBK_FALSE);
}
/**
 * Check whether the FCTA_activate is TRUE as expected
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__fcta_activated_pos)
{
   /** \arrange inputs for Enabled Flag ,Gear Position, DRIVE and braking configuration */
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled                        = FBK_TRUE;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake              = BMW_CTB_ACTIVATED;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake_with_braking = BMW_CTB_NOT_CONFIGURABLE;
   p_vehicle_data->prndl                                                    = PA_VEH_PRNDL_STATE_DRIVE;
   cta_input.bmw_ctb_input_signals.parking_context_active                   = true;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_front        = true;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_rear         = true;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_rear_braking = true;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_fcta_activate, FBK_TRUE);
}
/**
 * Check whether the FCTA_activate is FALSE as expected
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__fcta_activated_neg)
{
   /** \arrange inputs for Enabled Flag ,Gear Position, REVERSE and braking configuration */
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled                        = FBK_TRUE;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake              = BMW_CTB_ACTIVATED;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake_with_braking = BMW_CTB_NOT_CONFIGURABLE;
   p_vehicle_data->prndl                                                    = PA_VEH_PRNDL_STATE_REVERSE;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_fcta_activate, FBK_FALSE);
}
/**
 * Check whether the RCTA_activate is TRUE as expected
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__rcta_activated_pos)
{
   /** \arrange inputs for Enabled Flag ,Gear Position, REVERSE and braking configuration */
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled                        = FBK_TRUE;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake              = BMW_CTB_ACTIVATED;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake_with_braking = BMW_CTB_NOT_CONFIGURABLE;
   p_vehicle_data->prndl                                                    = PA_VEH_PRNDL_STATE_REVERSE;
   cta_input.bmw_ctb_input_signals.parking_context_active                   = true;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_front        = true;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_rear         = true;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_rear_braking = true;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_rcta_activate, FBK_TRUE);
}
/**
 * Check whether the RCTA_activate is FALSE as expected
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__rcta_activated_neg)
{
   /** \arrange inputs for Enabled Flag ,Gear Position, DRIVE and braking configuration */
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled                        = FBK_TRUE;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake              = BMW_CTB_ACTIVATED;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake_with_braking = BMW_CTB_NOT_CONFIGURABLE;
   p_vehicle_data->prndl                                                    = PA_VEH_PRNDL_STATE_DRIVE;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_rcta_activate, FBK_FALSE);
}
/**
 * Check whether the brake override is FALSE as expected
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Cta_Update_Flags__brake_override_neg)
{
   /** \arrange inputs Pedal Gradient < Threshold set */
   cta_input.bmw_ctb_input_signals.gradient_angle_acceleratorpedal = 15;
   /** \action executes Ctb State Machine */
   Cta_Update_Flags(&cta_input, p_cta_cals, &cta_instance.customer_calibration, &ctb_state_machine_flag, p_vehicle_data);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(ctb_state_machine_flag.f_brake_override, FBK_FALSE);
}
/**
 * Check state transition from Not Available to Ready
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__not_available_to_Ready)
{
   /** \arrange inputs fcta activation TRUE, initial state to READY, test mode to FALSE, speed check to TRUE */
   p_ctb_current_state                           = CTB_STATE_NOT_AVAILABLE;
   ctb_state_machine_flag.f_ctb_flag_ctb_enabled = FBK_TRUE;
   ctb_error                                     = BMW_CTA_NO_ERROR;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_READY);
}
/**
 * Check state transition from NOT AVAILABLE to READY
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__Not_available_to_ready_cta_enabled)
{
   /** \arrange inputs ctb flag TRUE */
   p_ctb_current_state                           = CTB_STATE_NOT_AVAILABLE;
   ctb_state_machine_flag.f_ctb_flag_ctb_enabled = FBK_FALSE;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_NOT_AVAILABLE);
}
/**
 * Check state transition from READY to NOT AVAILABLE
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__ready_to_not_available)
{
   /** \arrange inputs ctb flag TRUE*/
   p_ctb_current_state                           = CTB_STATE_READY;
   ctb_state_machine_flag.f_ctb_flag_ctb_enabled = FBK_FALSE;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_NOT_AVAILABLE);
}
/**
 * Check state transition from READY to NOT AVAILABLE
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__ready_to_not_available_cta_enabled)
{
   /** \arrange inputs ctb flag TRUE*/
   p_ctb_current_state                           = CTB_STATE_READY;
   ctb_state_machine_flag.f_ctb_flag_ctb_enabled = FBK_TRUE;
   ctb_error                                     = BMW_CTA_NO_ERROR;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_READY);
}
/**
 * Check state transition from READY to FCTA Degraded
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__ready_to_fcta_degraded_fcta_activate)
{
   /** \arrange inputs fcta activate TRUE, test mode FALSE, speed check TRUE */
   p_ctb_current_state                    = CTB_STATE_READY;
   ctb_state_machine_flag.f_fcta_activate = FBK_FALSE;
   ctb_state_machine_flag.f_ctb_test_mode = FBK_FALSE;
   ctb_state_machine_flag.f_speed_check   = FBK_TRUE;
   ctb_error                              = BMW_CTA_NO_ERROR;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_READY);
}
/**
 * Check state transition from READY to FCTA Degraded
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__ready_to_fcta_degraded_fcta_error)
{
   /** \arrange inputs fcta activate TRUE, test mode FALSE, speed check TRUE */
   p_ctb_current_state                    = CTB_STATE_READY;
   ctb_state_machine_flag.f_fcta_activate = FBK_TRUE;
   ctb_state_machine_flag.f_ctb_test_mode = FBK_FALSE;
   ctb_error                              = BMW_CTA_NO_ERROR;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_READY);
}
/**
 * Check state transition from READY to RCTA Degraded
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__ready_to_rcta_degraded_rcta_activate)
{
   /** \arrange inputs rcta activate TRUE, test mode FALSE, speed check TRUE */
   p_ctb_current_state                    = CTB_STATE_READY;
   ctb_state_machine_flag.f_rcta_activate = FBK_FALSE;
   ctb_state_machine_flag.f_ctb_test_mode = FBK_FALSE;
   ctb_error                              = BMW_CTA_NO_ERROR;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_READY);
}
/**
 * Check state transition from READY to RCTA Degraded
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__ready_to_rcta_degraded_test_mode)
{
   /** \arrange inputs rcta activate TRUE, test mode FALSE, speed check TRUE */
   p_ctb_current_state                    = CTB_STATE_READY;
   ctb_state_machine_flag.f_rcta_activate = FBK_TRUE;
   ctb_state_machine_flag.f_ctb_test_mode = FBK_TRUE;
   ctb_error                              = BMW_CTA_NO_ERROR;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_READY);
}
/**
 * Check state transition from READY to RCTA Degraded
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__ready_to_rcta_degraded_rcta_error)
{
   /** \arrange inputs rcta activate TRUE, test mode FALSE, speed check TRUE */
   p_ctb_current_state                    = CTB_STATE_READY;
   ctb_state_machine_flag.f_rcta_activate = FBK_TRUE;
   ctb_state_machine_flag.f_ctb_test_mode = FBK_FALSE;
   ctb_error                              = BMW_CTA_NO_ERROR;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_READY);
}
/**
 * Check state transition from READY to RCTA Active
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__ready_to_rcta_active)
{
   /** \arrange inputs rcta activate TRUE, test mode FALSE, speed check TRUE */
   p_ctb_current_state                    = CTB_STATE_READY;
   ctb_state_machine_flag.f_rcta_activate = FBK_TRUE;
   ctb_state_machine_flag.f_ctb_test_mode = FBK_FALSE;
   ctb_state_machine_flag.f_speed_check   = FBK_TRUE;
   ctb_error                              = BMW_CTA_NO_ERROR;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_RCTA_ACTIVE);
}
/**
 * Check state transition from READY to RCTA Active
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__ready_to_rcta_active_test_mode)
{
   /** \arrange inputs rcta activate TRUE, test mode FALSE, speed check TRUE */
   p_ctb_current_state                    = CTB_STATE_READY;
   ctb_state_machine_flag.f_rcta_activate = FBK_TRUE;
   ctb_state_machine_flag.f_ctb_test_mode = FBK_TRUE;
   ctb_state_machine_flag.f_speed_check   = FBK_TRUE;
   ctb_error                              = BMW_CTA_NO_ERROR;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_READY);
}
/**
 * Check state transition from READY to RCTA Active
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__ready_to_rcta_active_rcta_error)
{
   /** \arrange inputs rcta activate TRUE, test mode FALSE, speed check TRUE */
   p_ctb_current_state                    = CTB_STATE_READY;
   ctb_state_machine_flag.f_rcta_activate = FBK_TRUE;
   ctb_state_machine_flag.f_ctb_test_mode = FBK_FALSE;
   ctb_state_machine_flag.f_speed_check   = FBK_TRUE;
   ctb_error                              = BMW_CTA_ERROR;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_ERROR);
}
/**
 * Check state transition from READY to FCTA Active
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__ready_to_fcta_active_test_mode)
{
   /** \arrange inputs fcta activate TRUE, test mode FALSE, speed check TRUE */
   p_ctb_current_state                    = CTB_STATE_READY;
   ctb_state_machine_flag.f_fcta_activate = FBK_TRUE;
   ctb_state_machine_flag.f_ctb_test_mode = FBK_TRUE;
   ctb_state_machine_flag.f_speed_check   = FBK_TRUE;
   ctb_error                              = BMW_CTA_NO_ERROR;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_READY);
}
/**
 * Check state transition from READY to FCTA Active
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__ready_to_fcta_active_fcta_error)
{
   /** \arrange inputs fcta activate TRUE, test mode FALSE, speed check TRUE */
   p_ctb_current_state                    = CTB_STATE_READY;
   ctb_state_machine_flag.f_fcta_activate = FBK_TRUE;
   ctb_state_machine_flag.f_ctb_test_mode = FBK_FALSE;
   ctb_state_machine_flag.f_speed_check   = FBK_TRUE;
   ctb_error                              = BMW_CTA_ERROR;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_ERROR);
}
/**
 * Check state transition from READY to FCTA Active
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__ready_to_fcta_active)
{
   /** \arrange inputs fcta activation TRUE, initial state to READY, test mode to FALSE, speed check to TRUE */
   p_ctb_current_state                    = CTB_STATE_READY;
   ctb_state_machine_flag.f_fcta_activate = FBK_TRUE;
   ctb_state_machine_flag.f_ctb_test_mode = FBK_FALSE;
   ctb_state_machine_flag.f_speed_check   = FBK_TRUE;
   ctb_error                              = BMW_CTA_NO_ERROR;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_FCTA_ACTIVE);
}
/**
 * Check state transition from  RCTA Active to READY
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__rcta_active_to_ready)
{
   /** \arrange inputs CTB enabled TRUE, activation TRUE, initial state to rcta_active, Also setting speed check hys to TRUE
    * reverse gear engaged*/
   p_ctb_current_state                      = CTB_STATE_RCTA_ACTIVE;
   ctb_state_machine_flag.f_rcta_activate   = FBK_TRUE;
   ctb_state_machine_flag.f_ctb_test_mode   = FBK_FALSE;
   ctb_error                                = BMW_CTA_NO_ERROR;
   ctb_state_machine_flag.f_speed_check_hys = FBK_TRUE;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_READY);
}
/**
 * Check state transition from  RCTA Active to READY
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__rcta_active_to_ready_rcta_error)
{
   /** \arrange inputs CTB enabled TRUE, activation TRUE, initial state to rcta_active, Also setting speed check hys to TRUE
    * reverse gear engaged*/
   p_ctb_current_state                      = CTB_STATE_RCTA_ACTIVE;
   ctb_state_machine_flag.f_rcta_activate   = FBK_FALSE;
   ctb_state_machine_flag.f_ctb_test_mode   = FBK_TRUE;
   ctb_error                                = BMW_CTA_NO_ERROR;
   ctb_state_machine_flag.f_speed_check_hys = FBK_TRUE;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_READY);
}
/**
 * Check state transition from  RCTA Active to READY
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__rcta_active_to_ready_test_mode)
{
   /** \arrange inputs CTB enabled TRUE, activation TRUE, initial state to rcta active and test mode to TRUE*/
   p_ctb_current_state                    = CTB_STATE_RCTA_ACTIVE;
   ctb_state_machine_flag.f_rcta_activate = FBK_TRUE;
   ctb_state_machine_flag.f_ctb_test_mode = FBK_TRUE;
   ctb_error                              = BMW_CTA_NO_ERROR;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_READY);
}
/**
 * Check state transition from  FCTA Active to READY Speed Check Fail
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__fcta_active_to_ready)
{
   /** \arrange inputs FCTA activation TRUE, initial state to fcta_active, speed check hys TRUE
    * and drive gear engaged*/
   p_ctb_current_state                      = CTB_STATE_FCTA_ACTIVE;
   ctb_state_machine_flag.f_fcta_activate   = FBK_TRUE;
   ctb_state_machine_flag.f_speed_check_hys = FBK_TRUE;
   ctb_error                                = BMW_CTA_NO_ERROR;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_READY);
}
/**
 * Check state transition from  RCTA Active to READY
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__fcta_active_to_ready_fcta_error)
{
   /** \arrange inputs CTB enabled TRUE, activation TRUE, initial state to rcta_active, Also setting speed check hys to TRUE
    * reverse gear engaged*/
   p_ctb_current_state                           = CTB_STATE_FCTA_ACTIVE;
   ctb_state_machine_flag.f_ctb_flag_ctb_enabled = FBK_TRUE;
   ctb_state_machine_flag.f_fcta_activate        = FBK_FALSE;
   ctb_state_machine_flag.f_ctb_test_mode        = FBK_TRUE;
   ctb_error                                     = BMW_CTA_NO_ERROR;
   ctb_state_machine_flag.f_speed_check_hys      = FBK_TRUE;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_READY);
}
/**
 * Check state transition from  FCTA Active to READY Test mode activation
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__fcta_active_to_ready_test_mode)
{
   /** \arrange inputs CTB enabled TRUE, activation TRUE, initial state to fcta active,  Test mode activated*/
   p_ctb_current_state                    = CTB_STATE_FCTA_ACTIVE;
   ctb_state_machine_flag.f_fcta_activate = FBK_FALSE;
   ctb_state_machine_flag.f_ctb_test_mode = FBK_TRUE;
   ctb_error                              = BMW_CTA_NO_ERROR;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_READY);
}
/**
 * Check state transition from Ready to Error
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__ready_to_error_rcta_fcta_error)
{
   /** \arrange inputs setting ctb errors to CRITICAL */
   p_ctb_current_state = CTB_STATE_READY;
   ctb_error           = BMW_CTA_NO_ERROR;
   // ctb_error                           = BMW_CTA_ERROR;
   ctb_state_machine_flag.f_speed_check = FBK_FALSE;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_READY);
}
/**
 * Check state transition from  RCTA Active to Error
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__rcta_active_to_error)
{
   /** \arrange inputs CTB enabled TRUE, activation TRUE, initial state to rcta active and rcta error to CRITICAL*/
   p_ctb_current_state                    = CTB_STATE_RCTA_ACTIVE;
   ctb_state_machine_flag.f_rcta_activate = FBK_TRUE;
   ctb_error                              = BMW_CTA_ERROR;
   ctb_state_machine_flag.f_speed_check   = FBK_TRUE;
   ctb_state_machine_flag.f_ctb_test_mode = FBK_FALSE;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_ERROR);
}
/**
 * Check state transition from  RCTA Active to Error
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__rcta_active_to_error_test_mode)
{
   /** \arrange inputs CTB enabled TRUE, activation TRUE, initial state to rcta active and rcta error to CRITICAL*/
   p_ctb_current_state                           = CTB_STATE_RCTA_ACTIVE;
   ctb_state_machine_flag.f_rcta_activate        = FBK_TRUE;
   ctb_state_machine_flag.f_ctb_flag_ctb_enabled = FBK_TRUE;
   ctb_error                                     = BMW_CTA_NO_ERROR;
   ctb_state_machine_flag.f_speed_check          = FBK_TRUE;
   ctb_state_machine_flag.f_ctb_test_mode        = FBK_TRUE;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_NE(p_ctb_current_state, CTB_STATE_ERROR);
}
/**
 * Check state transition from  FCTA Active to Error
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__fcta_active_to_error)
{
   /** \arrange inputs CTB enabled TRUE, activation TRUE, initial state to fcta active and rcta error to CRITICAL*/
   p_ctb_current_state                    = CTB_STATE_FCTA_ACTIVE;
   ctb_state_machine_flag.f_fcta_activate = FBK_TRUE;
   ctb_error                              = BMW_CTA_ERROR;
   ctb_state_machine_flag.f_speed_check   = FBK_TRUE;
   ctb_state_machine_flag.f_ctb_test_mode = FBK_FALSE;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_ERROR);
}
/**
 * Check state transition from  FCTA Active to Error
 * \uts{} \sdd{n/a} \testtype{negative}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__fcta_active_to_error_Test_mode)
{
   /** \arrange inputs CTB enabled TRUE, activation TRUE, initial state to fcta active and rcta error to CRITICAL*/
   p_ctb_current_state = CTB_STATE_FCTA_ACTIVE;
   ctb_error           = BMW_CTA_NO_ERROR;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_NE(p_ctb_current_state, CTB_STATE_ERROR);
}
/**
 * Check state transition from FCTA Error to Ready
 * \uts{} \sdd{n/a} \testtype{positive}
 */
TEST_F(Cta_State_Machine_Test, Ctb_State_Machine__error_to_ready)
{
   /** \arrange inputs CTB enabled TRUE, activation TRUE, initial state to fcta active and errors are no error*/
   p_ctb_current_state = CTB_STATE_ERROR;
   ctb_error           = BMW_CTA_NO_ERROR;
   /** \action executes Ctb State Machine */
   Cta_State_Machine(&p_ctb_current_state, &ctb_error, ctb_state_machine_flag);
   /** \assert expect the cta switch to be TRUE */
   EXPECT_EQ(p_ctb_current_state, CTB_STATE_READY);
}
