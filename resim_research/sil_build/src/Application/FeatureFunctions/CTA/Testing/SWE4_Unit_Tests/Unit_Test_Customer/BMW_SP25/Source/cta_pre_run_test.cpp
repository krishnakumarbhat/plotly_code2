/**
 * @file cta_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SRR5 CTA pre run
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42097}
 */

#include "cta_pre_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "cta_pre_run.c"
#include "cta_types.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_vehicle_in.h"
}


/**
 * Check whether the CTA zone is constructued correctly.
 * \uts{CSCSA-42099} \sdd{SF-3956} \testtype{positive}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Construct_Zone__verify_the_zone_setting)
{
   /** \arrange simplify test by setting angles for zone definition to 0.5 pi and 0.75 PI */
   cta_instance.calibration.k_cta_angles_zone_definition[1u] = (3.0f / 4.0f * PI);
   cta_instance.calibration.k_cta_angles_zone_definition[0u] = (1.0f / 4.0f * PI);
   p_vehicle_data->host_length                               = 4.0f;
   /** \action executes pre run initialization */
   Cta_Construct_Zone(&(cta_instance.core_input.cta_zone), &cta_instance.calibration, p_vehicle_data);

   /** \assert expect that mapping of zone points is done correctly */
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[0].x,
                   cta_instance.calibration.k_cta_butterfly_long[0u] - p_vehicle_data->host_length);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[0].y, cta_instance.calibration.k_cta_butterfly_lat[0u]);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[1].x,
                   -1.0f * p_vehicle_data->host_length + cta_instance.calibration.k_cta_butterfly_long[1u]);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[1].y, cta_instance.calibration.k_cta_butterfly_lat[1u]);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[2].x,
                   cta_instance.core_input.cta_zone.points[1].x - cta_instance.core_input.cta_zone.points[2].y);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[2].y, cta_instance.calibration.k_cta_butterfly_lat[2u]);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[3].y, cta_instance.calibration.k_cta_max_length_fov);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[3].x,
                   cta_instance.core_input.cta_zone.points[2].x - cta_instance.calibration.k_cta_max_length_fov);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.size, cta_instance.calibration.k_cta_amount_butterfly_points_in_use);
}

/**
 * Check whether the radar position is mapped correctly in case that debug mode is disabled.
 * \uts{CSCSA-42100} \sdd{SF-3945} \testtype{positive}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Pre_Run__debug_mode_disabled_rcta_enabled)
{
   /** \arrange set up initial values for switch flag and radar position */
   cta_instance.calibration.k_cta_DEBUG_MODE                                = FBK_FALSE;
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled                        = FBK_TRUE;
   cta_instance.core_input.p_pa_data                                        = NULL;
   *Ctb_Current_State                                                       = CTB_STATE_RCTA_ACTIVE;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake              = BMW_CTB_ACTIVATED;
   data.vehicle_data.host_speed                                             = 1.0f;
   data.vehicle_data.prndl                                                  = PA_VEH_PRNDL_STATE_REVERSE;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_front        = true;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_rear         = true;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_rear_braking = true;
   cta_input.bmw_ctb_input_signals.parking_context_active                   = true;
   /** \action executes input setting function for core */
   Cta_Pre_Run(&cta_instance, &cta_input, &fbk_output, &pt_output);

   /** \assert expect that mapping of radar position and switch flag is done correctly */
   EXPECT_EQ(cta_instance.core_input.f_cta_switch, cta_input.bmw_ctb_coding_parameters.c_ctb_enabled);
   EXPECT_TRUE(cta_instance.core_input.p_pa_data != nullptr);
}

/**
 * Check that the pre run is filling the required core input.
 * \uts{CSCSA-42101} \sdd{SF-3945} \testtype{positive}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Pre_Run__check_that_core_input_is_set_up)
{
   /** \arrange set up initial values for switch flag and radar position */
   cta_instance.calibration.k_cta_DEBUG_MODE                                = FBK_FALSE;
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled                        = FBK_TRUE;
   cta_instance.core_input.p_pa_data                                        = NULL;
   *Ctb_Current_State                                                       = CTB_STATE_RCTA_ACTIVE;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake              = BMW_CTB_ACTIVATED;
   data.vehicle_data.host_speed                                             = 1.0f;
   data.vehicle_data.prndl                                                  = PA_VEH_PRNDL_STATE_REVERSE;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_front        = true;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_rear         = true;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_rear_braking = true;
   cta_input.bmw_ctb_input_signals.parking_context_active                   = true;

   /** \action executes overal pre run */
   Cta_Pre_Run(&cta_instance, &cta_input, &fbk_output, &pt_output);

   /** \assert expect radar position, level calibration, zone, and other metadata are set accordingly */
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[0].x, cta_instance.calibration.k_cta_butterfly_long[0u]);
   EXPECT_EQ(cta_instance.core_input.f_cta_switch, cta_input.bmw_ctb_coding_parameters.c_ctb_enabled);
   EXPECT_TRUE(cta_instance.core_input.p_pa_data != nullptr);
   for (uint8_t mode_idx = FBK_ZERO_UINT; mode_idx < CTA_NUM_MODES; mode_idx++)
   {
      for (uint8_t level_idx = FBK_ZERO_UINT; level_idx < CTA_NUM_CRIT_LEVEL; level_idx++)
      {
         EXPECT_FLOAT_EQ(cta_instance.core_input.ttc_criticality_level[mode_idx][level_idx],
                         cta_instance.calibration.k_cta_ttc_criticality_level[mode_idx][level_idx]);
      }
   }
}

/**
 * Check whether the radar position is mapped correctly in case that debug mode is disabled.
 * \uts{CSCSA-70393} \sdd{SF-3945} \testtype{positive}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Pre_Run__debug_mode_disabled_fcta_enabled)
{
   /** \arrange set up initial values for switch flag and radar position */
   cta_instance.calibration.k_cta_DEBUG_MODE                                = FBK_FALSE;
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled                        = FBK_TRUE;
   cta_instance.core_input.p_pa_data                                        = NULL;
   *Ctb_Current_State                                                       = CTB_STATE_FCTA_ACTIVE;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake              = BMW_CTB_ACTIVATED;
   data.vehicle_data.host_speed                                             = 1.0f;
   data.vehicle_data.prndl                                                  = PA_VEH_PRNDL_STATE_DRIVE;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_front        = true;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_rear         = true;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_rear_braking = true;
   cta_input.bmw_ctb_input_signals.parking_context_active                   = true;

   /** \action executes input setting function for core */
   Cta_Pre_Run(&cta_instance, &cta_input, &fbk_output, &pt_output);

   /** \assert expect that mapping of radar position and switch flag is done correctly */
   EXPECT_EQ(cta_instance.core_input.f_cta_switch, cta_input.bmw_ctb_coding_parameters.c_ctb_enabled);
   EXPECT_TRUE(cta_instance.core_input.p_pa_data != nullptr);
}

/**
 * Check whether the radar position is mapped correctly in case that debug mode is disabled.
 * \uts{CSCSA-70395} \sdd{SF-3945} \testtype{positive}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Pre_Run__debug_mode_disabled_rcta_degrade)
{
   /** \arrange set up initial values for switch flag and radar position */
   cta_instance.calibration.k_cta_DEBUG_MODE                   = FBK_FALSE;
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled           = FBK_TRUE;
   cta_instance.core_input.p_pa_data                           = NULL;
   *Ctb_Current_State                                          = CTB_STATE_ACTIVE;
   cta_input.bmw_ctb_input_signals.setting_cross_traffic_brake = BMW_CTB_ACTIVATED;
   cta_instance.calibration.k_cta_DEBUG_MODE                   = FBK_TRUE;
   cta_instance.calibration.k_cta_switch                       = FBK_TRUE;
   data.vehicle_data.host_speed                                = -1.0f;
   data.vehicle_data.prndl                                     = PA_VEH_PRNDL_STATE_REVERSE;
   /** \action executes input setting function for core */
   Cta_Pre_Run(&cta_instance, &cta_input, &fbk_output, &pt_output);

   /** \assert expect that mapping of radar position and switch flag is done correctly */
   EXPECT_EQ(cta_instance.core_input.f_cta_switch, FBK_TRUE);
   EXPECT_TRUE(cta_instance.core_input.p_pa_data != nullptr);
}
