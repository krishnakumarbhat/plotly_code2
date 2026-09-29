/**
 * @file lcda_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lcda_pre_run.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-43001}
 */

#include "lcda_pre_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "lcda_pre_run.c"
#include "lcda_types.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}


/**
 * Check that if LCDA is in trailer mode, only BSW is enabled by LCDA input and all other submodules are disabled.
 * \uts{CSCSA-43002} \sdd{SF-6836} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Pre_Run__only_enable_bsw_submodule_for_trailer_mode)
{
   /** \arrange Set up LCDA input such that trailer mode is enabled. */
   lcda_input.f_lcda_trailer_mode      = 1u;
   lcda_input.f_lcda_trailer_connected = 1u;

   lcda_input.f_lcda_enable_bsw = 1u;
   lcda_input.f_lcda_enable_cvw = 1u;
   lcda_input.f_lcda_enable_slc = 1u;
   lcda_input.f_lcda_enable_awa = 1u;

   /** \action Call Lcda_Pre_Run to fill LCDA core input from calibration values and LCDA input. */
   Lcda_Pre_Run(&lcda_instance, &lcda_input, &fbk_output);

   /** \assert Check that only BSW is enabled because of trailer mode. */
   EXPECT_TRUE(lcda_core_input.enabled_flags.f_bsw_enabled);
   EXPECT_FALSE(lcda_core_input.enabled_flags.f_cvw_enabled);
   EXPECT_FALSE(lcda_core_input.enabled_flags.f_slc_enabled);
   EXPECT_FALSE(lcda_core_input.enabled_flags.f_elc_enabled);
}

/**
 * Check that if LCDA is in GBT mode (china specific) the BSW zone is set to fixed VCS coordinates.
 * \uts{CSCSA-43003} \sdd{SF-6836} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Pre_Run__only_enable_bsw_submodule_for_trailer_mode2)
{
   /** \arrange Set up LCDA input such that GBT mode isenabled. */
   lcda_input.f_lcda_enable_bsw_GBT = FBK_TRUE;

   /** \action Call Lcda_Pre_Run to fill LCDA core input from calibration values and LCDA input. */
   Lcda_Pre_Run(&lcda_instance, &lcda_input, &fbk_output);

   /** \assert Check that BSW zone uses fixed VCS coordinates. */
   EXPECT_EQ(lcda_core_input.bsw_zone_calculation_mode, BSW_ZONE_CALC_FIXED_ZONE_VCS);
}

/**
 * Check that if LCDA is in trailer mode, only BSW is enabled by LCDA input and all other submodules are disabled.
 * \uts{CSCSA-70078} \sdd{SF-6836} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Pre_Run__only_enable_bsw_submodule_for_trailer_mode_Active)
{
   /** \arrange Set up LCDA input such that trailer mode is enabled. */
   lcda_input.f_lcda_trailer_mode                               = 1u;
   lcda_input.f_lcda_trailer_connected                          = 0u;
   lcda_input.lcda_coding_parameters.c_f_lcda_enabled           = FBK_TRUE;
   lcda_input.lcda_coding_parameters.c_f_lcda_enable_bsw        = FBK_ONE_UINT;
   lcda_input.lcda_coding_parameters.c_lcda_max_vel_upper_limit = 3;
   lcda_input.lcda_input_signals.vehicle_condition              = FAHREN;
   lcda_input.lcda_input_signals.vehicle_driving_direction      = VEHICLE_MOVES_FORWARD;
   lcda_input.lcda_input_signals.lcda_function_error            = LEVEL0;

   lcda_input.lcda_coding_parameters.c_min_curve_radii = 1.5;
   lcda_input.lcda_input_signals.curve_radii           = 2.5;
   lcda_input.f_lcda_enable                            = 1u;
   lcda_input.f_lcda_enable_bsw                        = 1u;
   lcda_input.f_lcda_enable_cvw                        = 1u;
   lcda_input.f_lcda_enable_slc                        = 1u;
   lcda_input.f_lcda_enable_awa                        = 1u;

   /** \action Call Lcda_Pre_Run to fill LCDA core input from calibration values and LCDA input. */
   Lcda_Pre_Run(&lcda_instance, &lcda_input, &fbk_output);

   /** \assert Check that only BSW is enabled because of trailer mode. */
   EXPECT_TRUE(lcda_core_input.enabled_flags.f_lcda_enabled);
   EXPECT_TRUE(lcda_core_input.enabled_flags.f_bsw_enabled);
   EXPECT_TRUE(lcda_core_input.enabled_flags.f_cvw_enabled);
   EXPECT_TRUE(lcda_core_input.enabled_flags.f_slc_enabled);
   EXPECT_TRUE(lcda_core_input.enabled_flags.f_elc_enabled);
}


/**
 * Check that warn settings are set correctly for hmi mode late. Use feature input for mapping.
 * \uts{CSCSA-43004} \sdd{SF-6850} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Hmi_Warn_Settings__sets_correct_warn_settings_for_hmi_mode_late)
{
   /** \arrange Set up LCDA input and calibration values such that warn trigger mode late is used. */
   cals.k_lcda_use_default_warntrigger_hmi = 0u;
   lcda_input.lcda_warntrigger_hmi         = (uint8_t) LCDA_BMW_WARNTRIGGER_LATE;

   /** \action Call Lcda_Set_Hmi_Warn_Settings to fill LCDA core input from calibration values and LCDA input. */
   Lcda_Set_Hmi_Warn_Settings(&lcda_core_input, &lcda_input, &cals);

   /** \assert Check that warn settings are set according to hmi mode late. */
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.bsw_len_factor, cals.k_bsw_warntrigger_late);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, cals.k_cvw_ttc + cals.k_cvw_warntrigger_late);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.slc_ttc_thres_lon, cals.k_slc_critical_lon_ttc + cals.k_slc_warntrigger_TTC_lon_late);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.slc_ttc_thres_lat, cals.k_slc_critical_lat_ttc + cals.k_slc_warntrigger_TTC_lat_late);
   EXPECT_FALSE(lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone);
}

/**
 * Check that warn settings are set correctly for hmi mode normal. For this the default setting via calibration shall be used.
 * \uts{CSCSA-43005} \sdd{SF-6850} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Hmi_Warn_Settings__sets_correct_warn_settings_for_hmi_mode_normal_use_default_by_cal)
{
   /** \arrange Set up LCDA input and calibration values such that hmi mode normal is used. */
   cals.k_lcda_use_default_warntrigger_hmi = 1u;
   cals.k_lcda_default_warntrigger_hmi     = (uint8_t) LCDA_BMW_WARNTRIGGER_NORMAL;

   /** \action Call Lcda_Set_Hmi_Warn_Settings to fill LCDA core input from calibration values and LCDA input. */
   Lcda_Set_Hmi_Warn_Settings(&lcda_core_input, &lcda_input, &cals);

   /** \assert Check that warn settings are set according to hmi mode normal. */
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.bsw_len_factor, LCDA_BMW_DEFAULT_BSW_ADJUSTMENT_FACTOR);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, cals.k_cvw_ttc);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.slc_ttc_thres_lon, cals.k_slc_critical_lon_ttc);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.slc_ttc_thres_lat, cals.k_slc_critical_lat_ttc);
   EXPECT_FALSE(lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone);
}

/**
 * Check that warn settings are set correctly for hmi mode early.
 * \uts{CSCSA-43006} \sdd{SF-6850} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Hmi_Warn_Settings__sets_correct_warn_settings_for_hmi_mode_early)
{
   /** \arrange Set up LCDA input and calibration values such that hmi mode 2 is used. */
   cals.k_lcda_use_default_warntrigger_hmi = 1u;
   cals.k_lcda_default_warntrigger_hmi     = (uint8_t) LCDA_BMW_WARNTRIGGER_EARLY;


   /** \action Call Lcda_Set_Hmi_Warn_Settings to fill LCDA core input from calibration values and LCDA input. */
   Lcda_Set_Hmi_Warn_Settings(&lcda_core_input, &lcda_input, &cals);

   /** \assert Check that warn settings are set according to hmi mode early. */
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.bsw_len_factor, cals.k_bsw_warntrigger_early);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, cals.k_cvw_ttc + cals.k_cvw_warntrigger_early);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.slc_ttc_thres_lon,
                   cals.k_slc_critical_lon_ttc + cals.k_slc_warntrigger_TTC_lon_early);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.slc_ttc_thres_lat,
                   cals.k_slc_critical_lat_ttc + cals.k_slc_warntrigger_TTC_lat_early);
   EXPECT_FALSE(lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone);
}

/**
 * Check that warn settings are set correctly for hmi mode very early. Here the calibration usage flag shall be overwritten because
 * the Warntrigger very early is given. \uts{CSCSA-43007} \sdd{SF-6850} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Hmi_Warn_Settings__sets_correct_warn_settings_for_hmi_mode_very_early)
{
   /** \arrange Set up LCDA input and calibration values such that hmi mode 2 is used. */
   cals.k_lcda_use_default_warntrigger_hmi = 1u;
   cals.k_lcda_default_warntrigger_hmi     = (uint8_t) LCDA_BMW_WARNTRIGGER_NORMAL;
   lcda_input.lcda_warntrigger_hmi         = (uint8_t) LCDA_BMW_WARNTRIGGER_VERY_EARLY;


   /** \action Call Lcda_Set_Hmi_Warn_Settings to fill LCDA core input from calibration values and LCDA input. */
   Lcda_Set_Hmi_Warn_Settings(&lcda_core_input, &lcda_input, &cals);

   /** \assert Check that warn settings are set according to hmi mode very early. */
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.bsw_len_factor, LCDA_BMW_DEFAULT_BSW_ADJUSTMENT_FACTOR);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, cals.k_cvw_ttc);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.slc_ttc_thres_lon, cals.k_slc_critical_lon_ttc);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.slc_ttc_thres_lat, cals.k_slc_critical_lat_ttc);
   EXPECT_TRUE(lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone);
}

/**
 * Check that warn settings are set to hmi mode normal for unknown hmi mode.
 * \uts{CSCSA-43008} \sdd{SF-6850} \testtype{negative}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Hmi_Warn_Settings__sets_warn_settings_to_hmi_mode_normal_for_unknown_mode)
{
   /** \arrange Set up LCDA input and calibration values such that hmi mode 5 which is not defined is used. */
   cals.k_lcda_use_default_warntrigger_hmi = 1u;
   cals.k_lcda_default_warntrigger_hmi     = 50u;

   /** \action Call Lcda_Pre_Run to fill LCDA core input from calibration values and LCDA input. */
   Lcda_Set_Hmi_Warn_Settings(&lcda_core_input, &lcda_input, &cals);

   /** \assert Check that warn settings are set according to hmi mode 1. */
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.bsw_len_factor, LCDA_BMW_DEFAULT_BSW_ADJUSTMENT_FACTOR);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, cals.k_cvw_ttc);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.slc_ttc_thres_lon, cals.k_slc_critical_lon_ttc);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.slc_ttc_thres_lat, cals.k_slc_critical_lat_ttc);
   EXPECT_FALSE(lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone);
}

/**
 * Check that guardrail data from tracker is filled into core input, if a guardrail is present.
 * \uts{CSCSA-43009} \sdd{SF-6847} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Guardrail_Data__fills_radar_guardrail_information_if_guardrail_is_present)
{
   /** \arrange Set up LCDA input with tracker guardrail data, such that a radar guardrail is present. */
   guardrail_data[FBK_SIDE_LEFT].f_present                                   = FBK_TRUE;
   guardrail_data[FBK_SIDE_LEFT].lat_pos                                     = 4.0f;
   guardrail_data[FBK_SIDE_LEFT].status                                      = PA_OBJ_STATUS_MATURE;
   lcda_core_input.p_pa_data                                                 = &data;
   Guardrail_Persistent.guardrail_data[FBK_SIDE_LEFT].radar.lateral_position = 4.0f;
   Guardrail_Persistent.stage_age[FBK_SIDE_LEFT]                             = cust_cals.k_bmw_sp25_guardrail_age_stage_thresh;
   /** \action Call Lcda_Set_Guardrail_Data to fill guardrail related data into LCDA core input from LCDA input. */
   Lcda_Set_Guardrail_Data(&lcda_core_input, &Guardrail_Persistent, &lcda_input, &cals, &cust_cals);

   /** \assert Check that guardrail data in core input is filled correctly from tracker data. */
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].radar.lateral_position, guardrail_data[FBK_SIDE_LEFT].lat_pos);
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].radar.status, LCDA_GUARDRAIL_VALID);
}

/**
 * Check that guardrail data is not used when disabled in CAF input.
 * \uts{CSCSA-43010} \sdd{SF-6847} \testtype{negative}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Guardrail_Data__does_not_fill_radar_guardrail_information_if_guardrail_is_disabled)
{
   /** \arrange Set up LCDA input with tracker guardrail data, such that a radar guardrail is present. */
   guardrail_data[FBK_SIDE_LEFT].f_present               = FBK_TRUE;
   guardrail_data[FBK_SIDE_LEFT].lat_pos                 = 4.0f;
   guardrail_data[FBK_SIDE_LEFT].status                  = PA_OBJ_STATUS_MATURE;
   lcda_input.f_lcda_enable_environment_plausibilization = 0u;

   /** \action Call Lcda_Set_Guardrail_Data to fill guardrail related data into LCDA core input from LCDA input. */
   Lcda_Set_Guardrail_Data(&lcda_core_input, &Guardrail_Persistent, &lcda_input, &cals, &cust_cals);

   /** \assert Check that guardrail data in core input is filled correctly from tracker data. */
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].radar.lateral_position, FBK_ZERO_F);
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].radar.status, LCDA_GUARDRAIL_INVALID);
}

/**
 * Check that guardrail data from camera data is filled into core input, if a camera guardrail of type structured is present for
 * first lane. \uts{CSCSA-43011} \sdd{SF-6847} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Guardrail_Data__fills_camera_guardrail_information_if_lane_type_structured_first_lane_matches)
{
   /** \arrange Set up LCDA input with camera guardrail data, such that lane type matches for first lane on both sides. */
   cam_data.lane_type_first_left                  = LCDA_CAMERA_LANE_TYPE_STRUCTURED;
   cam_data.lane_distance_first_left              = 4.0f;
   cam_data.lane_existance_probability_first_left = 87.0f;

   cam_data.lane_type_first_right                  = LCDA_CAMERA_LANE_TYPE_STRUCTURED;
   cam_data.lane_distance_first_right              = 3.0f;
   cam_data.lane_existance_probability_first_right = 86.0f;

   cals.k_lcda_f_enable_camera_based_guardrail = 1u;
   cust_cals.k_bmw_sp25_smooth_camera_signals  = 0u;

   /** \action Call Lcda_Set_Guardrail_Data to fill guardrail related data into LCDA core input from LCDA input. */
   Lcda_Set_Guardrail_Data(&lcda_core_input, &Guardrail_Persistent, &lcda_input, &cals, &cust_cals);

   /** \assert Check that guardrail data in core input is filled correctly from camera data. */
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.lateral_position, -cam_data.lane_distance_first_left);
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.confidence,
                   LCDA_CONVERT_FROM_PERCENTAGE(cam_data.lane_existance_probability_first_left));
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.status, LCDA_GUARDRAIL_VALID);

   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.lateral_position, -cam_data.lane_distance_first_right);
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.confidence,
                   LCDA_CONVERT_FROM_PERCENTAGE(cam_data.lane_existance_probability_first_right));
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.status, LCDA_GUARDRAIL_VALID);
}

/**
 * Check that guardrail data from camera data is filled into core input, if a camera guardrail of type road edge is present for
 * first lane. \uts{CSCSA-43012} \sdd{SF-6847} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Guardrail_Data__fills_camera_guardrail_information_if_lane_type_road_edge_first_lane_matches)
{
   /** \arrange Set up LCDA input with camera guardrail data, such that lane type matches for first lane on both sides. */
   cam_data.lane_type_first_left                  = LCDA_CAMERA_LANE_TYPE_ROAD_EDGE;
   cam_data.lane_distance_first_left              = 4.0f;
   cam_data.lane_existance_probability_first_left = 87.0f;

   cam_data.lane_type_first_right                  = LCDA_CAMERA_LANE_TYPE_ROAD_EDGE;
   cam_data.lane_distance_first_right              = 3.0f;
   cam_data.lane_existance_probability_first_right = 86.0f;

   cust_cals.k_bmw_sp25_smooth_camera_signals  = 0u;
   cals.k_lcda_f_enable_camera_based_guardrail = 1u;

   /** \action Call Lcda_Set_Guardrail_Data to fill guardrail related data into LCDA core input from LCDA input. */
   Lcda_Set_Guardrail_Data(&lcda_core_input, &Guardrail_Persistent, &lcda_input, &cals, &cust_cals);

   /** \assert Check that guardrail data in core input is filled correctly from camera data. */
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.lateral_position, -cam_data.lane_distance_first_left);
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.confidence,
                   LCDA_CONVERT_FROM_PERCENTAGE(cam_data.lane_existance_probability_first_left));
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.status, LCDA_GUARDRAIL_VALID);

   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.lateral_position, -cam_data.lane_distance_first_right);
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.confidence,
                   LCDA_CONVERT_FROM_PERCENTAGE(cam_data.lane_existance_probability_first_right));
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.status, LCDA_GUARDRAIL_VALID);
}

/**
 * Check that guardrail data from camera data is filled into core input, if a camera guardrail of type curb is present for first
 * lane. \uts{CSCSA-43013} \sdd{SF-6847} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Guardrail_Data__fills_camera_guardrail_information_if_lane_type_curb_first_lane_matches)
{
   /** \arrange Set up LCDA input with camera guardrail data, such that lane type matches for first lane on both sides. */
   cam_data.lane_type_first_left                  = LCDA_CAMERA_LANE_TYPE_CURB;
   cam_data.lane_distance_first_left              = 4.0f;
   cam_data.lane_existance_probability_first_left = 87.0f;

   cam_data.lane_type_first_right                  = LCDA_CAMERA_LANE_TYPE_CURB;
   cam_data.lane_distance_first_right              = 3.0f;
   cam_data.lane_existance_probability_first_right = 86.0f;

   cust_cals.k_bmw_sp25_smooth_camera_signals  = 0u;
   cals.k_lcda_f_enable_camera_based_guardrail = 1u;

   /** \action Call Lcda_Set_Guardrail_Data to fill guardrail related data into LCDA core input from LCDA input. */
   Lcda_Set_Guardrail_Data(&lcda_core_input, &Guardrail_Persistent, &lcda_input, &cals, &cust_cals);

   /** \assert Check that guardrail data in core input is filled correctly from camera data. */
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.lateral_position, -cam_data.lane_distance_first_left);
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.confidence,
                   LCDA_CONVERT_FROM_PERCENTAGE(cam_data.lane_existance_probability_first_left));
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.status, LCDA_GUARDRAIL_VALID);

   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.lateral_position, -cam_data.lane_distance_first_right);
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.confidence,
                   LCDA_CONVERT_FROM_PERCENTAGE(cam_data.lane_existance_probability_first_right));
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.status, LCDA_GUARDRAIL_VALID);
}

/**
 * Check that guardrail data from camera data is filled into core input, if a camera guardrail of type road structured is present
 * for second lane. \uts{CSCSA-43014} \sdd{SF-6847} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Guardrail_Data__fills_camera_guardrail_information_if_lane_type_structured_second_lane_matches)
{
   /** \arrange Set up LCDA input with camera guardrail data, such that lane type matches for second lane on both sides. */
   cam_data.lane_type_second_left                  = LCDA_CAMERA_LANE_TYPE_STRUCTURED;
   cam_data.lane_distance_second_left              = 4.0f;
   cam_data.lane_existance_probability_second_left = 87.0f;

   cam_data.lane_type_second_right                  = LCDA_CAMERA_LANE_TYPE_STRUCTURED;
   cam_data.lane_distance_second_right              = 3.0f;
   cam_data.lane_existance_probability_second_right = 86.0f;

   cust_cals.k_bmw_sp25_smooth_camera_signals  = 0u;
   cals.k_lcda_f_enable_camera_based_guardrail = 1u;

   /** \action Call Lcda_Set_Guardrail_Data to fill guardrail related data into LCDA core input from LCDA input. */
   Lcda_Set_Guardrail_Data(&lcda_core_input, &Guardrail_Persistent, &lcda_input, &cals, &cust_cals);

   /** \assert Check that guardrail data in core input is filled correctly from camera data. */
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.lateral_position, -cam_data.lane_distance_second_left);
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.confidence,
                   LCDA_CONVERT_FROM_PERCENTAGE(cam_data.lane_existance_probability_second_left));
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.status, LCDA_GUARDRAIL_VALID);

   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.lateral_position, -cam_data.lane_distance_second_right);
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.confidence,
                   LCDA_CONVERT_FROM_PERCENTAGE(cam_data.lane_existance_probability_second_right));
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.status, LCDA_GUARDRAIL_VALID);
}

/**
 * Check that guardrail data from camera data is filled into core input, if a camera guardrail of type road edge is present for
 * second lane. \uts{CSCSA-43015} \sdd{SF-6847} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Guardrail_Data__fills_camera_guardrail_information_if_lane_type_road_edge_second_lane_matches)
{
   /** \arrange Set up LCDA input with camera guardrail data, such that lane type matches for second lane on both sides. */
   cam_data.lane_type_second_left                  = LCDA_CAMERA_LANE_TYPE_ROAD_EDGE;
   cam_data.lane_distance_second_left              = 4.0f;
   cam_data.lane_existance_probability_second_left = 87.0f;

   cam_data.lane_type_second_right                  = LCDA_CAMERA_LANE_TYPE_ROAD_EDGE;
   cam_data.lane_distance_second_right              = 3.0f;
   cam_data.lane_existance_probability_second_right = 86.0f;

   cust_cals.k_bmw_sp25_smooth_camera_signals  = 0u;
   cals.k_lcda_f_enable_camera_based_guardrail = 1u;

   /** \action Call Lcda_Set_Guardrail_Data to fill guardrail related data into LCDA core input from LCDA input. */
   Lcda_Set_Guardrail_Data(&lcda_core_input, &Guardrail_Persistent, &lcda_input, &cals, &cust_cals);

   /** \assert Check that guardrail data in core input is filled correctly from camera data. */
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.lateral_position, -cam_data.lane_distance_second_left);
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.confidence,
                   LCDA_CONVERT_FROM_PERCENTAGE(cam_data.lane_existance_probability_second_left));
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.status, LCDA_GUARDRAIL_VALID);

   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.lateral_position, -cam_data.lane_distance_second_right);
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.confidence,
                   LCDA_CONVERT_FROM_PERCENTAGE(cam_data.lane_existance_probability_second_right));
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.status, LCDA_GUARDRAIL_VALID);
}

/**
 * Check that guardrail data from camera data is filled into core input, if a camera guardrail of type curb is present for second
 * lane. \uts{CSCSA-43016} \sdd{SF-6847} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Guardrail_Data__fills_camera_guardrail_information_if_lane_type_curb_second_lane_matches)
{
   /** \arrange Set up LCDA input with camera guardrail data, such that lane type matches for second lane on both sides. */
   cam_data.lane_type_second_left                  = LCDA_CAMERA_LANE_TYPE_CURB;
   cam_data.lane_distance_second_left              = 4.0f;
   cam_data.lane_existance_probability_second_left = 87.0f;

   cam_data.lane_type_second_right                  = LCDA_CAMERA_LANE_TYPE_CURB;
   cam_data.lane_distance_second_right              = 3.0f;
   cam_data.lane_existance_probability_second_right = 86.0f;

   cust_cals.k_bmw_sp25_smooth_camera_signals  = 0u;
   cals.k_lcda_f_enable_camera_based_guardrail = 1u;

   /** \action Call Lcda_Set_Guardrail_Data to fill guardrail related data into LCDA core input from LCDA input. */
   Lcda_Set_Guardrail_Data(&lcda_core_input, &Guardrail_Persistent, &lcda_input, &cals, &cust_cals);

   /** \assert Check that guardrail data in core input is filled correctly from camera data. */
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.lateral_position, -cam_data.lane_distance_second_left);
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.confidence,
                   LCDA_CONVERT_FROM_PERCENTAGE(cam_data.lane_existance_probability_second_left));
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.status, LCDA_GUARDRAIL_VALID);

   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.lateral_position, -cam_data.lane_distance_second_right);
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.confidence,
                   LCDA_CONVERT_FROM_PERCENTAGE(cam_data.lane_existance_probability_second_right));
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.status, LCDA_GUARDRAIL_VALID);
}

/**
 * Check that guardrail data from camera data is not filled into core input, if camera guardrail is disabled by calibration value.
 * \uts{CSCSA-43017} \sdd{SF-6847} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Guardrail_Data__does_not_fill_camera_guardrail_information_if_disabled_by_cal)
{
   /** \arrange Set up LCDA input with camera guardrail data, such that lane type matches for first lane on both sides, but camera
    * guardrail is disabled by calibration value. */
   cam_data.lane_type_first_left                  = LCDA_CAMERA_LANE_TYPE_STRUCTURED;
   cam_data.lane_distance_first_left              = 4.0f;
   cam_data.lane_existance_probability_first_left = 87.0f;

   cam_data.lane_type_first_right                  = LCDA_CAMERA_LANE_TYPE_STRUCTURED;
   cam_data.lane_distance_first_right              = 3.0f;
   cam_data.lane_existance_probability_first_right = 86.0f;

   cals.k_lcda_f_enable_camera_based_guardrail = 0u;

   /** \action Call Lcda_Set_Guardrail_Data to fill guardrail related data into LCDA core input from LCDA input. */
   Lcda_Set_Guardrail_Data(&lcda_core_input, &Guardrail_Persistent, &lcda_input, &cals, &cust_cals);

   /** \assert Check that guardrail data in core input is filled with default values only. */
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.lateral_position, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.confidence, FBK_ZERO_F);
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.status, LCDA_GUARDRAIL_INVALID);

   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.lateral_position, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.confidence, FBK_ZERO_F);
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.status, LCDA_GUARDRAIL_INVALID);
}

/**
 * Check that guardrail data from camera data is not filled into core input, if camera guardrail is disabled by CAF input value.
 * \uts{CSCSA-43018} \sdd{SF-6847} \testtype{negative}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Guardrail_Data__does_not_fill_camera_guardrail_information_if_disabled_by_caf_input)
{
   /** \arrange Set up LCDA input with camera guardrail data, such that lane type matches for first lane on both sides, but camera
    * guardrail is disabled by CAF value. */
   cam_data.lane_type_first_left                  = LCDA_CAMERA_LANE_TYPE_STRUCTURED;
   cam_data.lane_distance_first_left              = 4.0f;
   cam_data.lane_existance_probability_first_left = 87.0f;

   cam_data.lane_type_first_right                  = LCDA_CAMERA_LANE_TYPE_STRUCTURED;
   cam_data.lane_distance_first_right              = 3.0f;
   cam_data.lane_existance_probability_first_right = 86.0f;

   cals.k_lcda_f_enable_camera_based_guardrail           = 1u;
   lcda_input.f_lcda_enable_environment_plausibilization = 0u;

   /** \action Call Lcda_Set_Guardrail_Data to fill guardrail related data into LCDA core input from LCDA input. */
   Lcda_Set_Guardrail_Data(&lcda_core_input, &Guardrail_Persistent, &lcda_input, &cals, &cust_cals);

   /** \assert Check that guardrail data in core input is filled with default values only. */
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.lateral_position, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.confidence, FBK_ZERO_F);
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].camera.status, LCDA_GUARDRAIL_INVALID);

   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.lateral_position, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.confidence, FBK_ZERO_F);
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].camera.status, LCDA_GUARDRAIL_INVALID);
}

/**
 * Adjust the probability adjustment values such that the adjustment is triggered. At the same time, the adjusted value should
 * trigger a reset of the bad guardrail holding counters. \uts{CSCSA-43019} \sdd{SF-6847} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Guardrail_Data__adjust_probability_cal_such_that_holding_counter_is_reset)
{
   /** \arrange Set up LCDA input with tracker guardrail data, such that probability adjustment is triggered. */
   cust_cals.k_bmw_sp25_lowest_probabilty_percentage_cal_for_adjustment = 0.0f;
   cals.k_min_exist_prob_camera_guardrail                               = -1.0f;
   Guardrail_Persistent.bad_guardrail_holding_counter[FBK_SIDE_LEFT]    = 5u;
   Guardrail_Persistent.bad_guardrail_holding_counter[FBK_SIDE_RIGHT]   = 5u;

   /** \action Call Lcda_Set_Guardrail_Data to fill guardrail related data into LCDA core input from LCDA input. */
   Lcda_Set_Guardrail_Data(&lcda_core_input, &Guardrail_Persistent, &lcda_input, &cals, &cust_cals);

   /** \assert Check that bad guardrail holding counters are reset. */
   EXPECT_EQ(Guardrail_Persistent.bad_guardrail_holding_counter[FBK_SIDE_LEFT], 0U);
   EXPECT_EQ(Guardrail_Persistent.bad_guardrail_holding_counter[FBK_SIDE_RIGHT], 0U);
}

/**
 * Fault injection with invalid camera data should lead to default camera output. Check that camera guard rail status is invalid.
 * trigger a reset of the bad guardrail holding counters. \uts{CSCSA-43020} \sdd{SF-6847} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Guardrail_Data__fault_injection_invalid_camera_confidence)
{
   /** \arrange Set up LCDA input with tracker guardrail data, prepare fault injection data. */
   cals.k_lcda_f_enable_camera_based_guardrail                   = FBK_TRUE;
   lcda_input.f_lcda_enable_environment_plausibilization         = FBK_TRUE;
   lcda_input.camera_data->lane_type_first_left                  = LCDA_CAMERA_LANE_TYPE_STRUCTURED;
   lcda_input.camera_data->lane_existance_probability_first_left = -1.0f;

   /** \action Call Lcda_Set_Guardrail_Data to fill guardrail related data into LCDA core input from LCDA input. */
   Lcda_Set_Guardrail_Data(&lcda_core_input, &Guardrail_Persistent, &lcda_input, &cals, &cust_cals);

   /** \assert Check that camera guard rail is invalid. */
   EXPECT_EQ(Guardrail_Persistent.guardrail_data[0].camera.status, LCDA_GUARDRAIL_INVALID);
}

/**
 * Fault injection with invalid adjustment cal. This should lead to default camera output. Check that camera guard rail status is
 * invalid. trigger a reset of the bad guardrail holding counters. \uts{CSCSA-43021} \sdd{SF-6847} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Guardrail_Data__fault_injection_invalid_adjustment_cal)
{
   /** \arrange Set up LCDA input with tracker guardrail data, prepare fault injection data. */
   cust_cals.k_bmw_sp25_lowest_probabilty_percentage_cal_for_adjustment = FBK_ONE_F;

   /** \action Call Lcda_Set_Guardrail_Data to fill guardrail related data into LCDA core input from LCDA input. */
   Lcda_Set_Guardrail_Data(&lcda_core_input, &Guardrail_Persistent, &lcda_input, &cals, &cust_cals);

   /** \assert Check that camera guard rail is invalid. */
   EXPECT_EQ(Guardrail_Persistent.guardrail_data[0].camera.status, LCDA_GUARDRAIL_INVALID);
}


/**
 * Initialize the bmw_sp25 customer adapter. Here the camera data is already initialized to a valid instance.Pointers shall be set
 * to any address other than NULL and fallback flag shall be true. \uts{CSCSA-43022} \sdd{SF-6840} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Run_Init_Input__camera_data_already_initialized)
{
   /** \arrange set up camera data to an already initialized instance. */
   lcda_input.camera_data = &Dummy_Camera_Data;

   /** \action execute pre run initialization. */
   Lcda_Init_Input(&lcda_input);

   /** \assert check that fall back is enabled. */
   EXPECT_TRUE(lcda_input.f_lcda_enable_fallback);
}


/**
 * Initialize the bmw_sp25 customer adapter. Camera data is shall also be initialized. Pointers shall be set to any address other
 * than NULL and fallback flag shall be true. \uts{CSCSA-43023} \sdd{SF-6840} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Run_Init_Input__camera_data_not_initialized)
{
   /** \arrange set all pointers to NULL. */
   lcda_input.camera_data = NULL;

   /** \action execute pre run initialization. */
   Lcda_Init_Input(&lcda_input);

   /** \assert check that fall back is enabled. */
   EXPECT_TRUE(lcda_input.f_lcda_enable_fallback);
   EXPECT_TRUE(lcda_input.camera_data != NULL);
}


/**
 * Detect lane change over the lanelines on the vehicle left side.
 * \uts{CSCSA-43024} \sdd{SF-6930} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Lane_Change_Detection__lane_change_left)
{
   /** \arrange Set up camera data for lane change */
   Lane_Change_Counter[FBK_SIDE_LEFT]                   = 2u;
   Lane_Change_Counter[FBK_SIDE_RIGHT]                  = 0u;
   Camera_Lane_Plausibilisation_Counter[FBK_SIDE_LEFT]  = 1u;
   Camera_Lane_Plausibilisation_Counter[FBK_SIDE_RIGHT] = 1u;
   lcda_input.camera_data->lane_distance_first_left     = 0.2f;
   lcda_input.camera_data->lane_distance_first_right    = -1.8f;

   cust_cals.k_bmw_sp25_f_enable_lane_change_detection = FBK_TRUE;
   p_vehicle_data->host_speed                          = cust_cals.k_bmw_sp25_lane_change_detection_host_speed_min;

   /** \action execute lane change detection */
   Lcda_Lane_Change_Detection(&lcda_core_input, p_vehicle_data, &lcda_input, &cals, &cust_cals);

   /** \assert check that the correct lane change is detected */
   EXPECT_TRUE(lcda_core_input.f_lane_change[FBK_SIDE_LEFT]);
   EXPECT_FALSE(lcda_core_input.f_lane_change[FBK_SIDE_RIGHT]);
}

/**
 * Detect lane change over the lanelines on the vehicle right side.
 * \uts{CSCSA-43025} \sdd{SF-6930} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Lane_Change_Detection__lane_change_right)
{
   /** \arrange Set up camera data for lane change */
   Lane_Change_Counter[FBK_SIDE_LEFT]                   = 0u;
   Lane_Change_Counter[FBK_SIDE_RIGHT]                  = 2u;
   Camera_Lane_Plausibilisation_Counter[FBK_SIDE_LEFT]  = 1u;
   Camera_Lane_Plausibilisation_Counter[FBK_SIDE_RIGHT] = 1u;
   lcda_input.camera_data->lane_distance_first_left     = 1.8f;
   lcda_input.camera_data->lane_distance_first_right    = -0.2f;

   cust_cals.k_bmw_sp25_f_enable_lane_change_detection = FBK_TRUE;
   p_vehicle_data->host_speed                          = cust_cals.k_bmw_sp25_lane_change_detection_host_speed_min;

   /** \action execute lane change detection */
   Lcda_Lane_Change_Detection(&lcda_core_input, p_vehicle_data, &lcda_input, &cals, &cust_cals);

   /** \assert check that the correct lane change is detected */
   EXPECT_FALSE(lcda_core_input.f_lane_change[FBK_SIDE_LEFT]);
   EXPECT_TRUE(lcda_core_input.f_lane_change[FBK_SIDE_RIGHT]);
}

/**
 * Do not activate opposite side lane change when one lane change flag is still active.
 * \uts{CSCSA-43026} \sdd{SF-6930} \testtype{negative}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Lane_Change_Detection__lane_change_opposite_side_blocked)
{
   /** \arrange Set up camera data for lane change right but lane change left is still active */
   lcda_core_input.f_lane_change[FBK_SIDE_LEFT]  = FBK_TRUE;
   lcda_core_input.f_lane_change[FBK_SIDE_RIGHT] = FBK_FALSE;

   Lane_Change_Counter[FBK_SIDE_LEFT]  = 10u;
   Lane_Change_Counter[FBK_SIDE_RIGHT] = 2u;

   Camera_Lane_Plausibilisation_Counter[FBK_SIDE_LEFT]  = 1u;
   Camera_Lane_Plausibilisation_Counter[FBK_SIDE_RIGHT] = 1u;

   lcda_input.camera_data->lane_distance_first_left  = 1.8f;
   lcda_input.camera_data->lane_distance_first_right = -0.2f;

   cust_cals.k_bmw_sp25_f_enable_lane_change_detection = FBK_TRUE;
   p_vehicle_data->host_speed                          = cust_cals.k_bmw_sp25_lane_change_detection_host_speed_min;

   /** \action execute lane change detection */
   Lcda_Lane_Change_Detection(&lcda_core_input, p_vehicle_data, &lcda_input, &cals, &cust_cals);

   /** \assert check that the correct lane change is detected */
   EXPECT_TRUE(lcda_core_input.f_lane_change[FBK_SIDE_LEFT]);
   EXPECT_FALSE(lcda_core_input.f_lane_change[FBK_SIDE_RIGHT]);
}

/**
 * Keep lane change flag active for set amount of cycles.
 * \uts{CSCSA-43027} \sdd{SF-6930} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Lane_Change_Detection__keep_lane_change_active_left)
{
   /** \arrange Set up lane change data */
   lcda_core_input.f_lane_change[FBK_SIDE_LEFT]  = FBK_TRUE;
   lcda_core_input.f_lane_change[FBK_SIDE_RIGHT] = FBK_FALSE;

   Lane_Change_Counter[FBK_SIDE_LEFT]  = 3u;
   Lane_Change_Counter[FBK_SIDE_RIGHT] = 0u;

   Camera_Lane_Plausibilisation_Counter[FBK_SIDE_LEFT]  = 1u;
   Camera_Lane_Plausibilisation_Counter[FBK_SIDE_RIGHT] = 1u;

   lcda_input.camera_data->lane_distance_first_left  = 1.5f;
   lcda_input.camera_data->lane_distance_first_right = -1.5f;

   cust_cals.k_bmw_sp25_f_enable_lane_change_detection = FBK_TRUE;
   p_vehicle_data->host_speed                          = cust_cals.k_bmw_sp25_lane_change_detection_host_speed_min;

   /** \action execute lane change detection */
   Lcda_Lane_Change_Detection(&lcda_core_input, p_vehicle_data, &lcda_input, &cals, &cust_cals);

   /** \assert check that the correct lane change is detected */
   EXPECT_TRUE(lcda_core_input.f_lane_change[FBK_SIDE_LEFT]);
   EXPECT_FALSE(lcda_core_input.f_lane_change[FBK_SIDE_RIGHT]);
}

/**
 * Keep lane change flag active for set amount of cycles.
 * \uts{CSCSA-43028} \sdd{SF-6930} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Lane_Change_Detection__keep_lane_change_active_right)
{
   /** \arrange Set up lane change data */
   lcda_core_input.f_lane_change[FBK_SIDE_LEFT]  = FBK_FALSE;
   lcda_core_input.f_lane_change[FBK_SIDE_RIGHT] = FBK_TRUE;

   Lane_Change_Counter[FBK_SIDE_LEFT]  = 0u;
   Lane_Change_Counter[FBK_SIDE_RIGHT] = 3u;

   Camera_Lane_Plausibilisation_Counter[FBK_SIDE_LEFT]  = 1u;
   Camera_Lane_Plausibilisation_Counter[FBK_SIDE_RIGHT] = 1u;

   lcda_input.camera_data->lane_distance_first_left  = 1.5f;
   lcda_input.camera_data->lane_distance_first_right = -1.5f;

   cust_cals.k_bmw_sp25_f_enable_lane_change_detection = FBK_TRUE;
   p_vehicle_data->host_speed                          = cust_cals.k_bmw_sp25_lane_change_detection_host_speed_min;

   /** \action execute lane change detection */
   Lcda_Lane_Change_Detection(&lcda_core_input, p_vehicle_data, &lcda_input, &cals, &cust_cals);

   /** \assert check that the correct lane change is detected */
   EXPECT_FALSE(lcda_core_input.f_lane_change[FBK_SIDE_LEFT]);
   EXPECT_TRUE(lcda_core_input.f_lane_change[FBK_SIDE_RIGHT]);
}

/**
 * Turn off detected lane when leaving lane lines for set amount of cycles.
 * \uts{CSCSA-43029} \sdd{SF-6930} \testtype{negative}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Lane_Change_Detection__lane_change_reset)
{
   /** \arrange Set up lane change data */
   lcda_core_input.f_lane_change[FBK_SIDE_LEFT]  = FBK_TRUE;
   lcda_core_input.f_lane_change[FBK_SIDE_RIGHT] = FBK_FALSE;

   Lane_Change_Counter[FBK_SIDE_LEFT]  = 1u;
   Lane_Change_Counter[FBK_SIDE_RIGHT] = 0u;

   Camera_Lane_Plausibilisation_Counter[FBK_SIDE_LEFT]  = 1u;
   Camera_Lane_Plausibilisation_Counter[FBK_SIDE_RIGHT] = 1u;

   lcda_input.camera_data->lane_distance_first_left  = 1.5f;
   lcda_input.camera_data->lane_distance_first_right = -1.5f;

   cust_cals.k_bmw_sp25_f_enable_lane_change_detection = FBK_TRUE;
   p_vehicle_data->host_speed                          = cust_cals.k_bmw_sp25_lane_change_detection_host_speed_min;

   /** \action execute lane change detection */
   Lcda_Lane_Change_Detection(&lcda_core_input, p_vehicle_data, &lcda_input, &cals, &cust_cals);

   /** \assert check that the correct lane change is detected */
   EXPECT_FALSE(lcda_core_input.f_lane_change[FBK_SIDE_LEFT]);
   EXPECT_FALSE(lcda_core_input.f_lane_change[FBK_SIDE_RIGHT]);
}


/**
 * Count up plausibilisation counter on the left side
 * \uts{CSCSA-43030} \sdd{SF-6931} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Camera_Lane_Plausibilisation__counter_increase_left)
{
   /** \arrange Set up plausibilisation counter and camera data */
   Camera_Lane_Plausibilisation_Counter[FBK_SIDE_LEFT]           = 0u;
   lcda_input.camera_data->lane_existance_probability_first_left = 100.0f;

   /** \action execute lane plausibilisation */
   Lcda_Camera_Lane_Plausibilisation(&lcda_input, &cust_cals);

   /** \assert check the counter value */
   EXPECT_EQ(Camera_Lane_Plausibilisation_Counter[FBK_SIDE_LEFT], 1u);
}

/**
 * Count up plausibilisation counter on the right side
 * \uts{CSCSA-43031} \sdd{SF-6931} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Camera_Lane_Plausibilisation__counter_increase_right)
{
   /** \arrange Set up plausibilisation counter and camera data */
   Camera_Lane_Plausibilisation_Counter[FBK_SIDE_RIGHT]           = 0u;
   lcda_input.camera_data->lane_existance_probability_first_right = 100.0f;

   /** \action execute lane plausibilisation */
   Lcda_Camera_Lane_Plausibilisation(&lcda_input, &cust_cals);

   /** \assert check the counter value */
   EXPECT_EQ(Camera_Lane_Plausibilisation_Counter[FBK_SIDE_RIGHT], 1u);
}

/**
 * Count down plausibilisation counter on the left side
 * \uts{CSCSA-43032} \sdd{SF-6931} \testtype{negative}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Camera_Lane_Plausibilisation__counter_decrease_left)
{
   /** \arrange Set up plausibilisation counter and camera data */
   Camera_Lane_Plausibilisation_Counter[FBK_SIDE_LEFT]           = 1u;
   lcda_input.camera_data->lane_existance_probability_first_left = 0.0f;

   /** \action execute lane plausibilisation */
   Lcda_Camera_Lane_Plausibilisation(&lcda_input, &cust_cals);

   /** \assert check the counter value */
   EXPECT_EQ(Camera_Lane_Plausibilisation_Counter[FBK_SIDE_LEFT], 0u);
}

/**
 * Count down plausibilisation counter on the right side
 * \uts{CSCSA-43033} \sdd{SF-6931} \testtype{negative}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Camera_Lane_Plausibilisation__counter_decrease_right)
{
   /** \arrange Set up plausibilisation counter and camera data */
   Camera_Lane_Plausibilisation_Counter[FBK_SIDE_RIGHT]           = 1u;
   lcda_input.camera_data->lane_existance_probability_first_right = 0.0f;

   /** \action execute lane plausibilisation */
   Lcda_Camera_Lane_Plausibilisation(&lcda_input, &cust_cals);

   /** \assert check the counter value */
   EXPECT_EQ(Camera_Lane_Plausibilisation_Counter[FBK_SIDE_RIGHT], 0u);
}

/**
 * Do not count up plausibilisation counter after saturation.
 * \uts{CSCSA-43034} \sdd{SF-6931} \testtype{negative}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Camera_Lane_Plausibilisation__saturate_counters)
{
   /** \arrange Set up plausibilisation counter and camera data */
   Camera_Lane_Plausibilisation_Counter[FBK_SIDE_LEFT]            = cust_cals.k_bmw_sp25_camera_lane_plausibilisation_counter_max;
   Camera_Lane_Plausibilisation_Counter[FBK_SIDE_RIGHT]           = cust_cals.k_bmw_sp25_camera_lane_plausibilisation_counter_max;
   lcda_input.camera_data->lane_existance_probability_first_left  = 100.0f;
   lcda_input.camera_data->lane_existance_probability_first_right = 100.0f;

   /** \action execute lane plausibilisation */
   Lcda_Camera_Lane_Plausibilisation(&lcda_input, &cust_cals);

   /** \assert check the counter values */
   EXPECT_EQ(Camera_Lane_Plausibilisation_Counter[FBK_SIDE_LEFT], cust_cals.k_bmw_sp25_camera_lane_plausibilisation_counter_max);
   EXPECT_EQ(Camera_Lane_Plausibilisation_Counter[FBK_SIDE_RIGHT], cust_cals.k_bmw_sp25_camera_lane_plausibilisation_counter_max);
}

/**
 * Check that guardrail data from tracker is filled into core input, if a guardrail is present.
 * \uts{CSCSA-43035} \sdd{SF-6963} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Radar_Based_Guardrail__fills_radar_guardrail_information_if_guardrail_is_present)
{
   /** \arrange Set up LCDA input with tracker guardrail data, such that a radar guardrail is present. */
   guardrail_data[FBK_SIDE_LEFT].f_present                                   = FBK_TRUE;
   guardrail_data[FBK_SIDE_LEFT].lat_pos                                     = 4.0f;
   guardrail_data[FBK_SIDE_LEFT].status                                      = PA_OBJ_STATUS_MATURE;
   Guardrail_Persistent.guardrail_data[FBK_SIDE_LEFT].radar.lateral_position = 4.0f;
   Guardrail_Persistent.stage_age[FBK_SIDE_LEFT]                             = cust_cals.k_bmw_sp25_guardrail_age_stage_thresh;

   /** \action Call Lcda_Set_Guardrail_Data to fill guardrail related data into LCDA core input from LCDA input. */
   Lcda_Set_Radar_Based_Guardrail(&lcda_core_input, &Guardrail_Persistent, &lcda_input, &cals, &cust_cals);

   /** \assert Check that guardrail data in core input is filled correctly from tracker data. */
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].radar.lateral_position, guardrail_data[FBK_SIDE_LEFT].lat_pos);
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].radar.status, LCDA_GUARDRAIL_VALID);
}

/**
 * Check that guardrail data from tracker is not filled into core input, due stage age below threshold.
 * \uts{} \sdd{SF-6963} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Radar_Based_Guardrail__no_radar_guardrail_information_if_stage_age_below_thresh)
{
   /** \arrange Set up LCDA input with tracker guardrail data, such that a radar guardrail is present. */
   guardrail_data[FBK_SIDE_LEFT].f_present                                   = FBK_TRUE;
   guardrail_data[FBK_SIDE_LEFT].lat_pos                                     = 4.0f;
   guardrail_data[FBK_SIDE_LEFT].status                                      = PA_OBJ_STATUS_MATURE;
   Guardrail_Persistent.guardrail_data[FBK_SIDE_LEFT].radar.lateral_position = 4.0f;
   Guardrail_Persistent.stage_age[FBK_SIDE_LEFT]                             = 0u;

   /** \action Call Lcda_Set_Guardrail_Data to fill guardrail related data into LCDA core input from LCDA input. */
   Lcda_Set_Radar_Based_Guardrail(&lcda_core_input, &Guardrail_Persistent, &lcda_input, &cals, &cust_cals);

   /** \assert Check that guardrail data in core input is filled correctly from tracker data. */
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].radar.status, LCDA_GUARDRAIL_INVALID);
}


/**
 * Check that guardrail data is not used when disabled in CAF input.
 * \uts{CSCSA-43036} \sdd{SF-6963} \testtype{negative}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Radar_Based_Guardrail__does_not_fill_radar_guardrail_information_if_guardrail_is_disabled)
{
   /** \arrange Set up LCDA input with tracker guardrail data, such that a radar guardrail is present. */
   guardrail_data[FBK_SIDE_LEFT].f_present               = FBK_TRUE;
   guardrail_data[FBK_SIDE_LEFT].lat_pos                 = 4.0f;
   guardrail_data[FBK_SIDE_LEFT].status                  = PA_OBJ_STATUS_MATURE;
   lcda_input.f_lcda_enable_environment_plausibilization = 0u;

   /** \action Call Lcda_Set_Guardrail_Data to fill guardrail related data into LCDA core input from LCDA input. */
   Lcda_Set_Radar_Based_Guardrail(&lcda_core_input, &Guardrail_Persistent, &lcda_input, &cals, &cust_cals);

   /** \assert Check that guardrail data in core input is filled correctly from tracker data. */
   EXPECT_FLOAT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].radar.lateral_position, FBK_ZERO_F);
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].radar.status, LCDA_GUARDRAIL_INVALID);
}

/**
 * Check that guardrail data from camera data is filled, if a camera guardrail of type structured is present for first lane.
 * \uts{CSCSA-43037} \sdd{SF-6964} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test,
       Lcda_Compute_Camera_Lat_Pos_And_Confidence__fills_camera_guardrail_information_if_lane_type_structured_first_lane_matches)
{
   /** \arrange Set up LCDA input with camera guardrail data, such that lane type matches for first lane. */
   float32_T camera_lateral_position;
   float32_T camera_confidence;
   cam_data.lane_type_first_left                   = LCDA_CAMERA_LANE_TYPE_STRUCTURED;
   cam_data.lane_distance_first_left               = 4.0f;
   cam_data.lane_existance_probability_first_left  = 87.0f;
   cam_data.lane_type_second_left                  = LCDA_CAMERA_LANE_TYPE_STRUCTURED;
   cam_data.lane_distance_second_left              = 4.0f;
   cam_data.lane_existance_probability_second_left = 87.0f;


   /** \action Call Lcda_Compute_Camera_Lat_Pos_And_Confidence to fill guardrail related data from LCDA input. */
   Lcda_Compute_Camera_Lat_Pos_And_Confidence(&camera_lateral_position, &camera_confidence, cam_data.lane_type_first_left,
                                              cam_data.lane_distance_first_left, cam_data.lane_existance_probability_first_left,
                                              cam_data.lane_type_second_left, cam_data.lane_distance_second_left,
                                              cam_data.lane_existance_probability_second_left);

   /** \assert Check that guardrail data is filled correctly from camera data. */
   EXPECT_FLOAT_EQ(camera_lateral_position, -cam_data.lane_distance_first_left);
   EXPECT_FLOAT_EQ(camera_confidence, LCDA_CONVERT_FROM_PERCENTAGE(cam_data.lane_existance_probability_first_left));
}

/**
 * Check that guardrail data from camera data is filled, if a camera guardrail of type road edge is present for first lane.
 * \uts{CSCSA-43038} \sdd{SF-6964} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test,
       Lcda_Compute_Camera_Lat_Pos_And_Confidence__fills_camera_guardrail_information_if_lane_type_road_edge_first_lane_matches)
{
   /** \arrange Set up LCDA input with camera guardrail data, such that lane type matches for first lane. */
   float32_T camera_lateral_position;
   float32_T camera_confidence;
   cam_data.lane_type_first_left                   = LCDA_CAMERA_LANE_TYPE_ROAD_EDGE;
   cam_data.lane_distance_first_left               = 4.0f;
   cam_data.lane_existance_probability_first_left  = 87.0f;
   cam_data.lane_type_second_left                  = LCDA_CAMERA_LANE_TYPE_ROAD_EDGE;
   cam_data.lane_distance_second_left              = 4.0f;
   cam_data.lane_existance_probability_second_left = 87.0f;


   /** \action Call Lcda_Compute_Camera_Lat_Pos_And_Confidence to fill guardrail related data from LCDA input. */
   Lcda_Compute_Camera_Lat_Pos_And_Confidence(&camera_lateral_position, &camera_confidence, cam_data.lane_type_first_left,
                                              cam_data.lane_distance_first_left, cam_data.lane_existance_probability_first_left,
                                              cam_data.lane_type_second_left, cam_data.lane_distance_second_left,
                                              cam_data.lane_existance_probability_second_left);

   /** \assert Check that guardrail data is filled correctly from camera data. */
   EXPECT_FLOAT_EQ(camera_lateral_position, -cam_data.lane_distance_first_left);
   EXPECT_FLOAT_EQ(camera_confidence, LCDA_CONVERT_FROM_PERCENTAGE(cam_data.lane_existance_probability_first_left));
}

/**
 * Check that guardrail data from camera data is filled, if a camera guardrail of type curb is present for first lane.
 * \uts{CSCSA-43039} \sdd{SF-6964} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test,
       Lcda_Compute_Camera_Lat_Pos_And_Confidence__fills_camera_guardrail_information_if_lane_type_curb_first_lane_matches)
{
   /** \arrange Set up LCDA input with camera guardrail data, such that lane type matches for first lane. */
   float32_T camera_lateral_position;
   float32_T camera_confidence;
   cam_data.lane_type_first_left                   = LCDA_CAMERA_LANE_TYPE_CURB;
   cam_data.lane_distance_first_left               = 4.0f;
   cam_data.lane_existance_probability_first_left  = 87.0f;
   cam_data.lane_type_second_left                  = LCDA_CAMERA_LANE_TYPE_CURB;
   cam_data.lane_distance_second_left              = 4.0f;
   cam_data.lane_existance_probability_second_left = 87.0f;


   /** \action Call Lcda_Compute_Camera_Lat_Pos_And_Confidence to fill guardrail related data from LCDA input. */
   Lcda_Compute_Camera_Lat_Pos_And_Confidence(&camera_lateral_position, &camera_confidence, cam_data.lane_type_first_left,
                                              cam_data.lane_distance_first_left, cam_data.lane_existance_probability_first_left,
                                              cam_data.lane_type_second_left, cam_data.lane_distance_second_left,
                                              cam_data.lane_existance_probability_second_left);

   /** \assert Check that guardrail data is filled correctly from camera data. */
   EXPECT_FLOAT_EQ(camera_lateral_position, -cam_data.lane_distance_first_left);
   EXPECT_FLOAT_EQ(camera_confidence, LCDA_CONVERT_FROM_PERCENTAGE(cam_data.lane_existance_probability_first_left));
}

/**
 * Check that guardrail data from camera data is filled, if a camera guardrail of type road structured is present for second lane.
 * \uts{CSCSA-43040} \sdd{SF-6964} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test,
       Lcda_Compute_Camera_Lat_Pos_And_Confidence__fills_camera_guardrail_information_if_lane_type_structured_second_lane_matches)
{
   /** \arrange Set up LCDA input with camera guardrail data, such that lane type matches for second lane. */
   float32_T camera_lateral_position;
   float32_T camera_confidence;
   cam_data.lane_type_first_left                   = LCDA_CAMERA_LANE_TYPE_SIGNAL_NOT_FILLED;
   cam_data.lane_distance_first_left               = 4.0f;
   cam_data.lane_existance_probability_first_left  = 87.0f;
   cam_data.lane_type_second_left                  = LCDA_CAMERA_LANE_TYPE_CURB;
   cam_data.lane_distance_second_left              = 4.0f;
   cam_data.lane_existance_probability_second_left = 87.0f;


   /** \action Call Lcda_Compute_Camera_Lat_Pos_And_Confidence to fill guardrail related data from LCDA input. */
   Lcda_Compute_Camera_Lat_Pos_And_Confidence(&camera_lateral_position, &camera_confidence, cam_data.lane_type_first_left,
                                              cam_data.lane_distance_first_left, cam_data.lane_existance_probability_first_left,
                                              cam_data.lane_type_second_left, cam_data.lane_distance_second_left,
                                              cam_data.lane_existance_probability_second_left);

   /** \assert Check that guardrail data is filled correctly from camera data. */
   EXPECT_FLOAT_EQ(camera_lateral_position, -cam_data.lane_distance_first_left);
   EXPECT_FLOAT_EQ(camera_confidence, LCDA_CONVERT_FROM_PERCENTAGE(cam_data.lane_existance_probability_first_left));
}

/**
 * Check that guardrail data from camera data is filled, if a camera guardrail of type road edge is present for second lane.
 * \uts{CSCSA-43041} \sdd{SF-6964} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test,
       Lcda_Compute_Camera_Lat_Pos_And_Confidence__fills_camera_guardrail_information_if_lane_type_road_edge_second_lane_matches)
{
   /** \arrange Set up LCDA input with camera guardrail data, such that lane type matches for second lane. */
   float32_T camera_lateral_position;
   float32_T camera_confidence;
   cam_data.lane_type_first_left                   = LCDA_CAMERA_LANE_TYPE_SIGNAL_NOT_FILLED;
   cam_data.lane_distance_first_left               = 4.0f;
   cam_data.lane_existance_probability_first_left  = 87.0f;
   cam_data.lane_type_second_left                  = LCDA_CAMERA_LANE_TYPE_ROAD_EDGE;
   cam_data.lane_distance_second_left              = 4.0f;
   cam_data.lane_existance_probability_second_left = 87.0f;


   /** \action Call Lcda_Compute_Camera_Lat_Pos_And_Confidence to fill guardrail related data from LCDA input. */
   Lcda_Compute_Camera_Lat_Pos_And_Confidence(&camera_lateral_position, &camera_confidence, cam_data.lane_type_first_left,
                                              cam_data.lane_distance_first_left, cam_data.lane_existance_probability_first_left,
                                              cam_data.lane_type_second_left, cam_data.lane_distance_second_left,
                                              cam_data.lane_existance_probability_second_left);

   /** \assert Check that guardrail data is filled correctly from camera data. */
   EXPECT_FLOAT_EQ(camera_lateral_position, -cam_data.lane_distance_first_left);
   EXPECT_FLOAT_EQ(camera_confidence, LCDA_CONVERT_FROM_PERCENTAGE(cam_data.lane_existance_probability_first_left));
}

/**
 * Check that guardrail data from camera data is filled, if a camera guardrail of type curb is present for second lane.
 * \uts{CSCSA-43042} \sdd{SF-6964} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test,
       Lcda_Compute_Camera_Lat_Pos_And_Confidence__fills_camera_guardrail_information_if_lane_type_curb_second_lane_matches)
{
   /** \arrange Set up LCDA input with camera guardrail data, such that lane type matches for second lane. */
   float32_T camera_lateral_position;
   float32_T camera_confidence;
   cam_data.lane_type_first_left                   = LCDA_CAMERA_LANE_TYPE_SIGNAL_NOT_FILLED;
   cam_data.lane_distance_first_left               = 4.0f;
   cam_data.lane_existance_probability_first_left  = 87.0f;
   cam_data.lane_type_second_left                  = LCDA_CAMERA_LANE_TYPE_CURB;
   cam_data.lane_distance_second_left              = 4.0f;
   cam_data.lane_existance_probability_second_left = 87.0f;


   /** \action Call Lcda_Compute_Camera_Lat_Pos_And_Confidence to fill guardrail related data from LCDA input. */
   Lcda_Compute_Camera_Lat_Pos_And_Confidence(&camera_lateral_position, &camera_confidence, cam_data.lane_type_first_left,
                                              cam_data.lane_distance_first_left, cam_data.lane_existance_probability_first_left,
                                              cam_data.lane_type_second_left, cam_data.lane_distance_second_left,
                                              cam_data.lane_existance_probability_second_left);

   /** \assert Check that guardrail data is filled correctly from camera data. */
   EXPECT_FLOAT_EQ(camera_lateral_position, -cam_data.lane_distance_first_left);
   EXPECT_FLOAT_EQ(camera_confidence, LCDA_CONVERT_FROM_PERCENTAGE(cam_data.lane_existance_probability_first_left));
}

/**
 * Count up lane change counter on the left side
 * \uts{CSCSA-43043} \sdd{SF-6966} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Lane_Change_Counter__counter_increase_left)
{
   /** \arrange Set up lane change counter and camera data */
   float32_T lane_dist_first                                     = cust_cals.k_bmw_sp25_lane_change_dist_to_laneline_max;
   float32_T lane_width_raw                                      = cals.k_lcda_min_lane_width;
   uint8_t side                                                  = FBK_SIDE_LEFT;
   uint8_t opposite_side                                         = FBK_SIDE_RIGHT;
   boolean_T f_host_drives_over_laneline                         = FBK_TRUE;
   boolean_T f_lane_plausible                                    = FBK_TRUE;
   Lane_Change_Counter[side]                                     = 0u;
   lcda_input.camera_data->lane_existance_probability_first_left = 100.0f;

   /** \action execute lane plausibilisation */
   Lcda_Set_Lane_Change_Counter(&lcda_core_input, &cals, &cust_cals, lane_dist_first, lane_width_raw, side, opposite_side,
                                f_host_drives_over_laneline, f_lane_plausible);

   /** \assert check the counter value */
   EXPECT_EQ(Lane_Change_Counter[side], 1u);
}

/**
 * Count up lane change counter on the right side
 * \uts{CSCSA-43044} \sdd{SF-6966} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Lane_Change_Counter__counter_increase_right)
{
   /** \arrange Set up lane change counter and camera data */
   float32_T lane_dist_first                                     = cust_cals.k_bmw_sp25_lane_change_dist_to_laneline_max;
   float32_T lane_width_raw                                      = cals.k_lcda_min_lane_width;
   uint8_t side                                                  = FBK_SIDE_RIGHT;
   uint8_t opposite_side                                         = FBK_SIDE_LEFT;
   boolean_T f_host_drives_over_laneline                         = FBK_TRUE;
   boolean_T f_lane_plausible                                    = FBK_TRUE;
   Lane_Change_Counter[side]                                     = 0u;
   lcda_input.camera_data->lane_existance_probability_first_left = 100.0f;

   /** \action execute lane plausibilisation */
   Lcda_Set_Lane_Change_Counter(&lcda_core_input, &cals, &cust_cals, lane_dist_first, lane_width_raw, side, opposite_side,
                                f_host_drives_over_laneline, f_lane_plausible);

   /** \assert check the counter value */
   EXPECT_EQ(Lane_Change_Counter[side], 1u);
}

/**
 * Count down lane change counter on the left side
 * \uts{CSCSA-43045} \sdd{SF-6966} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Lane_Change_Counter__counter_decrease_left)
{
   /** \arrange Set up lane change counter and camera data */
   float32_T lane_dist_first                                     = cust_cals.k_bmw_sp25_lane_change_dist_to_laneline_max;
   float32_T lane_width_raw                                      = cals.k_lcda_min_lane_width;
   uint8_t side                                                  = FBK_SIDE_LEFT;
   uint8_t opposite_side                                         = FBK_SIDE_RIGHT;
   boolean_T f_host_drives_over_laneline                         = FBK_TRUE;
   boolean_T f_lane_plausible                                    = FBK_FALSE;
   Lane_Change_Counter[side]                                     = 1u;
   lcda_input.camera_data->lane_existance_probability_first_left = 100.0f;

   /** \action execute lane plausibilisation */
   Lcda_Set_Lane_Change_Counter(&lcda_core_input, &cals, &cust_cals, lane_dist_first, lane_width_raw, side, opposite_side,
                                f_host_drives_over_laneline, f_lane_plausible);

   /** \assert check the counter value */
   EXPECT_EQ(Lane_Change_Counter[side], 0u);
}

/**
 * Count down lane change counter on the right side
 * \uts{CSCSA-43046} \sdd{SF-6966} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Lane_Change_Counter__counter_decrease_right)
{
   /** \arrange Set up lane change counter and camera data */
   float32_T lane_dist_first                                     = cust_cals.k_bmw_sp25_lane_change_dist_to_laneline_max;
   float32_T lane_width_raw                                      = cals.k_lcda_min_lane_width;
   uint8_t side                                                  = FBK_SIDE_RIGHT;
   uint8_t opposite_side                                         = FBK_SIDE_LEFT;
   boolean_T f_host_drives_over_laneline                         = FBK_TRUE;
   boolean_T f_lane_plausible                                    = FBK_FALSE;
   Lane_Change_Counter[side]                                     = 1u;
   lcda_input.camera_data->lane_existance_probability_first_left = 100.0f;

   /** \action execute lane plausibilisation */
   Lcda_Set_Lane_Change_Counter(&lcda_core_input, &cals, &cust_cals, lane_dist_first, lane_width_raw, side, opposite_side,
                                f_host_drives_over_laneline, f_lane_plausible);

   /** \assert check the counter value */
   EXPECT_EQ(Lane_Change_Counter[side], 0u);
}
