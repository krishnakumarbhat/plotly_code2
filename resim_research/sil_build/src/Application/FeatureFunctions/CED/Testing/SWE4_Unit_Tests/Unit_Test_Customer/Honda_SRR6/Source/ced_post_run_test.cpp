/**
 * @file ced_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Honda_SRR6 CED post run
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41687}
 */

#include "ced_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ced_post_run.c"
#include "ced_types.h"
#include "fbk_macros.h"
}


/**
 * Check that post run initialization routine resets the honda_srr6 output accordingly.
 * \uts{CSCSA-41688} \sdd{SF-3472} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Post_Run_Init__check_no_fatal_faliure)
{
   /** \arrange set output to non default */

   /** \action Run function to test */

   /** \assert Verify output is set to default */
   EXPECT_NO_FATAL_FAILURE(Ced_Post_Run_Init(&ced_instance););
}

/**
 * Check if the OSE target travel directions are passed correctly.
 * \uts{CSCSA-41690} \sdd{SF-3501} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Map_Travel_Direction_To_Ose__check_all_directions)
{
   /** \arrange */

   /** \action test each case for position option. Use 3u to pass the defualt condition */
   Ose_Target_Travel_Direction_T honda_travel_direction_front_result = Ced_Map_Travel_Direction_To_Ose(FBK_SIDE_FRONT);
   Ose_Target_Travel_Direction_T honda_travel_direction_rear_result  = Ced_Map_Travel_Direction_To_Ose(FBK_SIDE_REAR);
   Ose_Target_Travel_Direction_T honda_travel_direction_undef_result = Ced_Map_Travel_Direction_To_Ose(FBK_SIDE_UNDEFINED);
   Ose_Target_Travel_Direction_T honda_travel_direction_null_result  = Ced_Map_Travel_Direction_To_Ose(3u);

   /** \assert verify result directions */
   EXPECT_EQ(honda_travel_direction_front_result, OSE_FRONT_DIRECTION);
   EXPECT_EQ(honda_travel_direction_rear_result, OSE_REAR_DIRECTION);
   EXPECT_EQ(honda_travel_direction_undef_result, OSE_UNDEF_DIRECTION);
   EXPECT_EQ(honda_travel_direction_null_result, OSE_UNDEF_DIRECTION);
}

/**
 * Check if the OSE alert travel directions are passed correctly.
 * \uts{CSCSA-41691} \sdd{SF-3496} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Map_Alert_Level_To_Ose__check_all_alerts)
{
   /** \arrange */

   /** \action test each case for alert option 8 */
   Ose_Alert_T honda_alert_level_result_no_alert            = Ced_Map_Alert_Level_To_Ose(CED_NO_ALERT);
   Ose_Alert_T honda_alert_level_result_active_level_1      = Ced_Map_Alert_Level_To_Ose(CED_ALERT_ACTIVE_LEVEL_1);
   Ose_Alert_T honda_alert_level_result_alert_qualification = Ced_Map_Alert_Level_To_Ose(CED_ALERT_QUALIFICATION);

   /** \assert verify result alerts */
   EXPECT_EQ(honda_alert_level_result_no_alert, OSE_NO_ALERT);
   EXPECT_EQ(honda_alert_level_result_active_level_1, OSE_ACTIVE_ALERT);
   EXPECT_EQ(honda_alert_level_result_alert_qualification, OSE_NO_ALERT);
}

/**
 * Check that update output functions runs correctly with no ced alerts set and wrong ttcs and cutstom ttc is below threshold.
 * \uts{CSCSA-41692} \sdd{SF-3500} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Post_Run__check_no_alerts_cust_ttc_below_threshold)
{
   /** \arrange input and core output data */
   ced_input.f_ced_enable                                             = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]      = 1u;
   p_vehicle_data->host_length                                        = 5.0f;
   ced_instance.core_output.ced_front_bumper_pos_long[FBK_SIDE_LEFT]  = -5.5f;
   ced_instance.core_output.ced_vcs_vel_rel_x[FBK_SIDE_LEFT]          = 2.0f;
   p_vehicle_data->host_length                                        = 5.0f;
   ced_instance.core_output.ced_front_bumper_pos_long[FBK_SIDE_RIGHT] = -5.5f;
   ced_instance.core_output.ced_vcs_vel_rel_x[FBK_SIDE_RIGHT]         = 2.0f;

   /* object direction should be less than 0, to improve branch coverage*/
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = -1.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_RIGHT] = 1.0f;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]          = 0u;
   /* object direction should be less than 0, to improve branch coverage*/
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                      = -1.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                       = 1u;
   ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_LEFT] = 0.0f;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                    = CED_ALERT_ACTIVE_LEVEL_1;

   /** \action Update function to test */
   Ced_Post_Run(&ced_instance, &ced_input, &ced_output);

   /** \assert Verify output is set correctly */
   EXPECT_EQ(ced_output.f_ced_enable, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(ced_output.CED_ttc_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ced_output.CED_ttc_left, FBK_ZERO_F);
   EXPECT_EQ(ced_output.CED_id_left, FBK_ONE_UINT);
   EXPECT_EQ(ced_output.CED_id_right, FBK_ONE_UINT);
   EXPECT_EQ(ced_output.CED_dir_left, OSE_FRONT_DIRECTION);
   EXPECT_EQ(ced_output.CED_dir_right, OSE_REAR_DIRECTION);
   EXPECT_EQ(ced_output.CED_object_predicted_lat_pos_left, FBK_ZERO_F);
   EXPECT_EQ(ced_output.CED_object_predicted_lat_pos_right, FBK_ONE_F);
}

/**
 * Check that update output functions runs correctly with no ced alerts set and wrong ttcs and cutstom ttc is below threshold.
 * \uts{CSCSA-41693} \sdd{SF-3500} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Post_Run__check_no_alerts_cust_ttc_above_threshold)
{
   /** \arrange input and core output data */
   ced_input.f_ced_enable                                             = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT]      = 1u;
   p_vehicle_data->host_length                                        = 5.0f;
   ced_instance.core_output.ced_front_bumper_pos_long[FBK_SIDE_LEFT]  = -55.0f;
   ced_instance.core_output.ced_vcs_vel_rel_x[FBK_SIDE_LEFT]          = 2.0f;
   p_vehicle_data->host_length                                        = 5.0f;
   ced_instance.core_output.ced_front_bumper_pos_long[FBK_SIDE_RIGHT] = -55.0f;
   ced_instance.core_output.ced_vcs_vel_rel_x[FBK_SIDE_RIGHT]         = 2.0f;

   /* object direction should be less than 0, to improve branch coverage*/
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = -1.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_RIGHT] = 1.0f;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]          = 0u;
   /* object direction should be less than 0, to improve branch coverage*/
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                      = -1.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                       = 1u;
   ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_LEFT] = 0.0f;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                    = CED_ALERT_ACTIVE_LEVEL_1;

   ced_instance.customer_calibration.k_ced_f_honda_use_alert_ttc_threshold = 1;
   /** \action Update function to test */
   Ced_Post_Run(&ced_instance, &ced_input, &ced_output);

   /** \assert Verify output is set correctly */
   EXPECT_EQ(ced_output.f_ced_enable, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(ced_output.CED_ttc_right, 100.0f);
   EXPECT_FLOAT_EQ(ced_output.CED_ttc_left, 100.0f);
   EXPECT_EQ(ced_output.CED_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.CED_id_right, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.CED_dir_left, OSE_UNDEF_DIRECTION);
   EXPECT_EQ(ced_output.CED_dir_right, OSE_UNDEF_DIRECTION);
   EXPECT_EQ(ced_output.CED_object_predicted_lat_pos_left, 100.0f);
   EXPECT_EQ(ced_output.CED_object_predicted_lat_pos_right, 100.0f);
}

/**
 * Check that update output functions runs correctly with no ced alerts set and wrong ttcs.
 * \uts{CSCSA-41694} \sdd{SF-3500} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Post_Run__check_ttc_disable_alert)
{
   /** \arrange input and core output data */
   ced_input.f_ced_enable                                        = 0u;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_RIGHT] = 1u;
   /* object direction should be less than 0, to improve branch coverage*/
   ced_instance.core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 3.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_RIGHT]                       = 1u;
   ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_RIGHT] = 1.0f;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT]                    = CED_ALERT_ACTIVE_LEVEL_1;
   ced_instance.core_output.ced_object_direction[FBK_SIDE_LEFT]          = 0u;
   /* object direction should be less than 0, to improve branch coverage*/
   ced_instance.core_output.ced_ttc[FBK_SIDE_LEFT]                      = 3.0f;
   ced_instance.core_output.ced_id[FBK_SIDE_LEFT]                       = 1u;
   ced_instance.core_output.ced_object_predicted_lat_pos[FBK_SIDE_LEFT] = 0.0f;
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]                    = CED_ALERT_ACTIVE_LEVEL_1;

   ced_instance.customer_calibration.k_ced_f_honda_use_alert_ttc_threshold = FBK_TRUE;

   /** \action Update function to test */
   Ced_Post_Run(&ced_instance, &ced_input, &ced_output);

   /** \assert Verify output is set correctly */
   EXPECT_EQ(ced_output.f_ced_enable, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.CED_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.CED_id_right, FBK_ZERO_UINT);
   EXPECT_EQ(ced_output.CED_dir_left, OSE_UNDEF_DIRECTION);
   EXPECT_EQ(ced_output.CED_dir_right, OSE_UNDEF_DIRECTION);
   EXPECT_EQ(ced_output.CED_object_predicted_lat_pos_left, 100.0f);
   EXPECT_EQ(ced_output.CED_object_predicted_lat_pos_right, 100.0f);
   EXPECT_EQ(ced_output.CED_alert_right, OSE_NO_ALERT);
   EXPECT_EQ(ced_output.CED_alert_left, OSE_NO_ALERT);
}

/**
 * Check that update output functions runs correctly while holdking is on.
 * \uts{CSCSA-41700} \sdd{SF-3500} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Post_Run__check_holding)
{
   /** \arrange honda output and core output data */
   p_vehicle_data->host_length                                       = 5.0f;
   ced_instance.core_output.ced_front_bumper_pos_long[FBK_SIDE_LEFT] = -5.5f;
   ced_instance.core_output.ced_vcs_vel_rel_x[FBK_SIDE_LEFT]         = 2.0f;

   ced_instance.core_output.ced_front_bumper_pos_long[FBK_SIDE_RIGHT] = -5.5f;
   ced_instance.core_output.ced_vcs_vel_rel_x[FBK_SIDE_RIGHT]         = 2.0f;

   ced_output.CED_dir_right                      = OSE_FRONT_DIRECTION;
   ced_output.CED_ttc_right                      = 1.5f;
   ced_output.CED_id_right                       = 3u;
   ced_output.CED_object_predicted_lat_pos_right = 2.5f;
   ced_output.CED_alert_right                    = OSE_ACTIVE_ALERT;

   ced_output.CED_dir_left                      = OSE_REAR_DIRECTION;
   ced_output.CED_ttc_left                      = 3.0f;
   ced_output.CED_id_left                       = 2u;
   ced_output.CED_object_predicted_lat_pos_left = 2.0f;
   ced_output.CED_alert_left                    = OSE_ACTIVE_ALERT;

   ced_input.f_ced_enable = 1u;

   ced_output.CED_f_hold_alert[FBK_SIDE_LEFT]  = FBK_TRUE;
   ced_output.CED_f_hold_alert[FBK_SIDE_RIGHT] = FBK_TRUE;

   /** \action Update function to test */
   Ced_Post_Run(&ced_instance, &ced_input, &ced_output);

   /** \assert Verify output is not changed */
   EXPECT_EQ(ced_output.f_ced_enable, FBK_ONE_UINT);
   EXPECT_FLOAT_EQ(ced_output.CED_ttc_right, 1.5f);
   EXPECT_FLOAT_EQ(ced_output.CED_ttc_left, 3.0f);
   EXPECT_EQ(ced_output.CED_id_left, 2u);
   EXPECT_EQ(ced_output.CED_id_right, 3u);
   EXPECT_EQ(ced_output.CED_dir_left, OSE_REAR_DIRECTION);
   EXPECT_EQ(ced_output.CED_dir_right, OSE_FRONT_DIRECTION);
   EXPECT_EQ(ced_output.CED_object_predicted_lat_pos_left, 2.0f);
   EXPECT_EQ(ced_output.CED_object_predicted_lat_pos_right, 2.5f);
}

/**
 * Check if honda custom ttc is calculated correctly.
 * \uts{CSCSA-41701} \sdd{CSCSA-70413} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Calculate_Honda_Custom_Ttc__check_right_ttc)
{
   /** \arrange honda output and core output data */
   p_vehicle_data->host_length                                        = 5.0f;
   ced_instance.core_output.ced_front_bumper_pos_long[FBK_SIDE_RIGHT] = -55.0f;
   ced_instance.core_output.ced_vcs_vel_rel_x[FBK_SIDE_RIGHT]         = 2.0f;
   uint8_t side_index                                                 = FBK_SIDE_RIGHT;

   /** \action Update function to test */
   Ced_Calculate_Honda_Custom_Ttc(&ced_output, &ced_instance.core_output, &ced_instance.customer_calibration, &data, side_index);

   /** \assert Verify output is not changed */
   EXPECT_EQ(ced_output.CED_honda_custom_ttc[FBK_SIDE_RIGHT], 25.0f);
}

/**
 * Check if honda custom ttc is calculated correctly.
 * \uts{CSCSA-41702} \sdd{CSCSA-70413} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Calculate_Honda_Custom_Ttc__check_left_ttc)
{
   /** \arrange honda output and core output data */
   p_vehicle_data->host_length                                       = 5.0f;
   ced_instance.core_output.ced_front_bumper_pos_long[FBK_SIDE_LEFT] = 55.0f;
   ced_instance.core_output.ced_vcs_vel_rel_x[FBK_SIDE_LEFT]         = -2.0f;
   uint8_t side_index                                                = FBK_SIDE_LEFT;

   /** \action Update function to test */
   Ced_Calculate_Honda_Custom_Ttc(&ced_output, &ced_instance.core_output, &ced_instance.customer_calibration, &data, side_index);

   /** \assert Verify output is not changed */
   EXPECT_EQ(ced_output.CED_honda_custom_ttc[FBK_SIDE_LEFT], FBK_ZERO_F);
}

/**
 * Test the setting of e-ratch signals. Here wide zone (Level 2) is tested. Right object is out of e-latch zone (expeced no alert),
 * left in e-latch zone (expect alert). \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Process_Eratch_Signals__standard_zone_left_in_right_out)
{
   /** \arrange honda output and core output data */
   ced_output.CED_alert_right = OSE_ACTIVE_ALERT;
   ced_output.CED_alert_left  = OSE_ACTIVE_ALERT;

   ced_instance.core_output.ced_object_closest_lat_dist_predicted[FBK_SIDE_LEFT]  = 1.9f;
   ced_instance.core_output.ced_object_closest_lat_dist_predicted[FBK_SIDE_RIGHT] = 2.5f;

   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]  = 0;
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT] = 1;

   ced_input.ew_elatch_sense_stt = HONDA_EW_STANDARD_ZONE;

   /** \action Update function to test */
   Ced_Process_Eratch_Signals(&ced_output, &ced_input, &ced_instance.core_output, &ced_instance.customer_calibration, &data);

   /** \assert Verify output is not changed */
   EXPECT_EQ(ced_output.CED_eratch_alert_left, E_RATCH_ACTIVE_ALERT);
   EXPECT_EQ(ced_output.CED_eratch_alert_right, E_RATCH_NO_ALERT);
}

/**
 * Test the setting of e-ratch signals. Here short zone (Level 1) is tested. Right object is in of e-latch zone (expeced alert),
 * left is out of e-latch zone (expect no alert). \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Process_Eratch_Signals__short_zone_left_out_right_in)
{
   /** \arrange honda output and core output data */
   ced_output.CED_alert_right = OSE_ACTIVE_ALERT;
   ced_output.CED_alert_left  = OSE_ACTIVE_ALERT;

   ced_instance.core_output.ced_object_closest_lat_dist_predicted[FBK_SIDE_LEFT]  = 1.9f;
   ced_instance.core_output.ced_object_closest_lat_dist_predicted[FBK_SIDE_RIGHT] = 1.0f;

   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]  = 0;
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT] = 1;

   ced_input.ew_elatch_sense_stt = HONDA_EW_SHORT_ZONE;

   /** \action Update function to test */
   Ced_Process_Eratch_Signals(&ced_output, &ced_input, &ced_instance.core_output, &ced_instance.customer_calibration, &data);

   /** \assert Verify output is not changed */
   EXPECT_EQ(ced_output.CED_eratch_alert_left, E_RATCH_NO_ALERT);
   EXPECT_EQ(ced_output.CED_eratch_alert_right, E_RATCH_ACTIVE_ALERT);
}

/**
 * Test the setting of e-ratch signals. Here holding is tested, core alerts are off and duration is higher than threshold. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Process_Eratch_Signals__holding_both_sides_duration_over_ths)
{
   /** \arrange honda output and core output data */
   ced_output.CED_alert_right                                                     = OSE_NO_ALERT;
   ced_output.CED_alert_left                                                      = OSE_NO_ALERT;
   ced_instance.core_output.ced_object_closest_lat_dist_predicted[FBK_SIDE_LEFT]  = 2.5f;
   ced_instance.core_output.ced_object_closest_lat_dist_predicted[FBK_SIDE_RIGHT] = 3.0f;
   ced_output.CED_eratch_alert_left                                               = E_RATCH_ACTIVE_ALERT;
   ced_output.CED_eratch_alert_right                                              = E_RATCH_ACTIVE_ALERT;
   ced_output.CED_current_eratch_alert_duration[FBK_SIDE_LEFT]                    = 2.0f;
   ced_output.CED_current_eratch_alert_duration[FBK_SIDE_RIGHT]                   = 2.0f;

   ced_instance.customer_calibration.k_honda_min_eratch_alert_duration = 1.5f;

   ced_instance.core_output.ced_index[FBK_SIDE_LEFT]  = 0;
   ced_instance.core_output.ced_index[FBK_SIDE_RIGHT] = 1;

   ced_input.ew_elatch_sense_stt = HONDA_EW_SHORT_ZONE;

   /** \action Update function to test */
   Ced_Process_Eratch_Signals(&ced_output, &ced_input, &ced_instance.core_output, &ced_instance.customer_calibration, &data);

   /** \assert Verify alerts not triggered */
   EXPECT_EQ(ced_output.CED_eratch_alert_left, E_RATCH_NO_ALERT);
   EXPECT_EQ(ced_output.CED_eratch_alert_right, E_RATCH_NO_ALERT);
}

/**
 * Test the setting of e-ratch signals. Here holding is tested, core alerts are off and duration is too low on both sides. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Process_Eratch_Signals__holding_both_sides_duration_too_low)
{
   /** \arrange honda output and core output data */
   ced_output.CED_alert_right                                   = OSE_NO_ALERT;
   ced_output.CED_alert_left                                    = OSE_NO_ALERT;
   ced_output.CED_eratch_alert_left                             = E_RATCH_ACTIVE_ALERT;
   ced_output.CED_eratch_alert_right                            = E_RATCH_ACTIVE_ALERT;
   ced_output.CED_current_eratch_alert_duration[FBK_SIDE_LEFT]  = 1.0f;
   ced_output.CED_current_eratch_alert_duration[FBK_SIDE_RIGHT] = 1.0f;

   ced_instance.customer_calibration.k_honda_min_eratch_alert_duration = 1.5f;

   ced_instance.core_output.ced_object_closest_lat_dist_predicted[FBK_SIDE_LEFT]  = 2.5f;
   ced_instance.core_output.ced_object_closest_lat_dist_predicted[FBK_SIDE_RIGHT] = 3.0f;

   ced_input.ew_elatch_sense_stt = HONDA_EW_SHORT_ZONE;

   /** \action Update function to test */
   Ced_Process_Eratch_Signals(&ced_output, &ced_input, &ced_instance.core_output, &ced_instance.customer_calibration, &data);

   /** \assert Verify alerts on */
   EXPECT_EQ(ced_output.CED_eratch_alert_left, E_RATCH_ACTIVE_ALERT);
   EXPECT_EQ(ced_output.CED_eratch_alert_right, E_RATCH_ACTIVE_ALERT);
}

/**
 * Test the setting of e-ratch signals. Here holding is tested, last cycle alerts was off. \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Process_Eratch_Signals__holding_both_sides_last_alert_off)
{
   /** \arrange honda output and core output data */
   ced_output.CED_alert_right                                                     = OSE_NO_ALERT;
   ced_output.CED_alert_left                                                      = OSE_NO_ALERT;
   ced_output.CED_eratch_alert_left                                               = E_RATCH_NO_ALERT;
   ced_output.CED_eratch_alert_right                                              = E_RATCH_NO_ALERT;
   ced_instance.core_output.ced_object_closest_lat_dist_predicted[FBK_SIDE_LEFT]  = 2.5f;
   ced_instance.core_output.ced_object_closest_lat_dist_predicted[FBK_SIDE_RIGHT] = 3.0f;

   ced_input.ew_elatch_sense_stt = HONDA_EW_SHORT_ZONE;

   /** \action Update function to test */
   Ced_Process_Eratch_Signals(&ced_output, &ced_input, &ced_instance.core_output, &ced_instance.customer_calibration, &data);

   /** \assert Verify alerts not set */
   EXPECT_EQ(ced_output.CED_eratch_alert_left, E_RATCH_NO_ALERT);
   EXPECT_EQ(ced_output.CED_eratch_alert_right, E_RATCH_NO_ALERT);
}

/**
 * Test if the custom ttc lgoic can be disabled with calibration \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Check_Custom_Ttc_Is_In_Range__disabled_by_cal)
{
   /** \arrange honda output and core output data */
   boolean_T result;
   ced_instance.customer_calibration.k_ced_f_honda_use_alert_ttc_threshold = FBK_FALSE;

   /** \action Update function to test */
   result = Ced_Check_Custom_Ttc_Is_In_Range(&ced_output, &ced_instance.customer_calibration, FBK_SIDE_LEFT);

   /** \assert Verify ttc in range */
   EXPECT_TRUE(result);
}

/**
 * Test if the custom ttc lgoic can be disabled with calibration \uts{}
 * \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Test, Ced_Check_Long_Position__value_within_range)
{
   /** \arrange honda output and core output data */
   boolean_T result;

   ced_instance.core_output.ced_front_bumper_pos_long[FBK_SIDE_LEFT] = -30.0;
   data.vehicle_data.host_length                                     = 5.0;

   /** \action Update function to test */
   result = Ced_Check_Long_Position(&data, &ced_instance.core_output, &ced_instance.customer_calibration, FBK_SIDE_LEFT);

   /** \assert Verify ttc in range */
   EXPECT_TRUE(result);
}