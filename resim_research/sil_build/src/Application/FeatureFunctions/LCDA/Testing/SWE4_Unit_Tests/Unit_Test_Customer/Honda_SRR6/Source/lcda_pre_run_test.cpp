/**
 * @file lcda_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Honda_SRR6 LCDA pre run
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-43110}
 */

#include "lcda_pre_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "lcda_honda_instance.h"
#include "lcda_pre_run.c"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}


/**
 * Check that criticality mode is set.
 * \uts{CSCSA-43111} \sdd{SF-6836} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Pre_Run__crtiticality_general)
{
   /** \arrange Set up LCDA calibration and output. */

   Lcda_Get_Lcda_Honda_Instance()->bsw_alert_left  = 1u;
   Lcda_Get_Lcda_Honda_Instance()->bsw_alert_right = 1u;

   /** \action Call function to test. */
   Lcda_Pre_Run(&lcda_instance, &lcda_input, &fbk_output);

   /** \assert Check that flags are set correctly. */
   EXPECT_EQ(lcda_core_input.cvw_crit_mode[FBK_SIDE_LEFT], CVW_CRIT_LONG_DIST);
   EXPECT_EQ(lcda_core_input.cvw_crit_mode[FBK_SIDE_RIGHT], CVW_CRIT_LONG_DIST);
}

/**
 * Check that criticality mode is set.
 * \uts{CSCSA-43111} \sdd{SF-6836} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Pre_Run__dynamic_cvw_ttc_threshold)
{
   /** \arrange Set up LCDA calibration and output. */

   Lcda_Get_Lcda_Honda_Instance()->bsw_alert_left  = 1u;
   Lcda_Get_Lcda_Honda_Instance()->bsw_alert_right = 1u;
   lcda_input.cvw_range_stt                        = RANGE_STT_EARLY;
   float32_T lcda_honda_cvw_ttc_const;
   float32_T cvw_ttc_speed_factor;

   lcda_honda_cvw_ttc_const = cals.k_lcda_cvw_ttc_const[((uint8_t) lcda_input.cvw_range_stt) - FBK_ONE_UINT];
   cvw_ttc_speed_factor = (FBK_ONE_F / (2.0f * cals.k_lcda_cvw_ttc_accel[((uint8_t) lcda_input.cvw_range_stt) - FBK_ONE_UINT]));

   /** \action Call function to test. */
   Lcda_Pre_Run(&lcda_instance, &lcda_input, &fbk_output);

   /** \assert Check that flags are set correctly. */
   EXPECT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, lcda_honda_cvw_ttc_const);
   EXPECT_EQ(lcda_core_input.warn_settings.cvw_ttc_speed_factor, cvw_ttc_speed_factor);
}

/**
 * Check that guardrail data is set
 * \uts{CSCSA-43114} \sdd{SF-6847} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Guardrail_Data__present)
{
   /** \arrange Set up LCDA calibration and output. */
   data.guardrail_data[FBK_SIDE_LEFT].f_present  = FBK_TRUE;
   data.guardrail_data[FBK_SIDE_LEFT].status     = PA_OBJ_STATUS_MATURE;
   data.guardrail_data[FBK_SIDE_LEFT].lat_pos    = 3.0f;
   data.guardrail_data[FBK_SIDE_RIGHT].f_present = FBK_TRUE;
   data.guardrail_data[FBK_SIDE_RIGHT].status    = PA_OBJ_STATUS_MATURE;
   data.guardrail_data[FBK_SIDE_RIGHT].lat_pos   = 3.0f;

   /** \action Call function to test. */
   Lcda_Pre_Run(&lcda_instance, &lcda_input, &fbk_output);

   /** \assert Check that flags are set correctly. */
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].radar.lateral_position, 3.0f);
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].radar.confidence, LCDA_GUARDRAIL_HIGH_EXIST_PROB);
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].radar.status, LCDA_GUARDRAIL_VALID);
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].radar.lateral_position, 3.0f);
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].radar.confidence, LCDA_GUARDRAIL_HIGH_EXIST_PROB);
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].radar.status, LCDA_GUARDRAIL_VALID);
}

/**
 * Check that guardrail data is set, object is not mature
 * \uts{CSCSA-43115} \sdd{SF-6847} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Guardrail_Data__present_obj_new)
{
   /** \arrange Set up LCDA calibration and output. */
   data.guardrail_data[FBK_SIDE_LEFT].f_present  = FBK_TRUE;
   data.guardrail_data[FBK_SIDE_LEFT].status     = PA_OBJ_STATUS_NEW;
   data.guardrail_data[FBK_SIDE_LEFT].lat_pos    = 3.0f;
   data.guardrail_data[FBK_SIDE_RIGHT].f_present = FBK_TRUE;
   data.guardrail_data[FBK_SIDE_RIGHT].status    = PA_OBJ_STATUS_NEW;
   data.guardrail_data[FBK_SIDE_RIGHT].lat_pos   = 3.0f;

   /** \action Call function to test. */
   Lcda_Pre_Run(&lcda_instance, &lcda_input, &fbk_output);

   /** \assert Check that flags are set correctly. */
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].radar.lateral_position, 3.0f);
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].radar.confidence, LCDA_GUARDRAIL_LOW_EXIST_PROB);
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_LEFT].radar.status, LCDA_GUARDRAIL_VALID);
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].radar.lateral_position, 3.0f);
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].radar.confidence, LCDA_GUARDRAIL_LOW_EXIST_PROB);
   EXPECT_EQ(lcda_core_input.guardrail_data[FBK_SIDE_RIGHT].radar.status, LCDA_GUARDRAIL_VALID);
}
