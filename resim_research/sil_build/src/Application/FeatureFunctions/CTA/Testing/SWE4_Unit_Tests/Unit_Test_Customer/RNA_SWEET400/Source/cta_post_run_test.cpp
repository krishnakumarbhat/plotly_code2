/*===================================================================*\
* Copyright 2004, Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential.
*--------------------------------------------------------------------
*
* Description:
*
* Applicable Standards (in order of precedence: highest first):
*
* Deviations from Delco C Coding standards:
*
*
\*===================================================================*/
/*============================================*\
* MACROS
\*===========================================================================*/

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-70399}
 */

#include "cta_post_run_test.hpp"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "cta_post_run.c"
#include "cta_struct_initializer.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
}


#ifndef NDEBUG
/**
 * Test if the assertion is thrown while null pointers are passed;
 * \uts{CSCSA-70400} \sdd{SF-4020} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Calculate_Relevant_Target_Corners__Persistent_Pointer_Null)
{
   /** \arrange */

   /** \action */

   /** \assert */
   EXPECT_DEATH({ Cta_Calculate_Relevant_Target_Corners(NULL, &cta_instance, p_vehicle_data, FBK_SIDE_LEFT); }, ".*p_target_"
                                                                                                                "corners.*");
}
#endif //! NDEBUG

/**
 * Test if the output is correctly reset;
 * \uts{CSCSA-70401} \sdd{SF-4021} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Reset_Output__general_test)
{
   /** \arrange Set up random output */
   cta_output.cta_id_right                 = 3;
   cta_output.cta_ttc_right                = 2.0f;
   cta_output.cta_intersectionX_right      = -1.0f;
   cta_output.cta_radialDistance_right     = 2.0f;
   cta_output.cta_objPoseX_right           = 21.0024205f;
   cta_output.cta_objPoseY_right           = 145.0f;
   cta_output.cta_objVelocityX_right       = 56.0f;
   cta_output.cta_objVelocityY_right       = 54.0f;
   cta_output.f_cta_alert_right            = FBK_ONE_UINT;
   cta_output.f_cta_warn_right             = FBK_ONE_UINT;
   cta_output.f_cta_prefill_req_right      = FBK_ONE_UINT;
   cta_output.f_cta_braking_req_right      = FBK_ONE_UINT;
   cta_output.f_cta_hold_supp_right        = FBK_ONE_UINT;
   cta_output.RCTA_Criticality_level_right = FBK_ONE_UINT;
   cta_output.cta_heading_rear_left        = 0.4f;

   cta_output.cta_id_left                 = 2;
   cta_output.cta_ttc_left                = 3.0f;
   cta_output.cta_intersectionX_left      = 2.0f;
   cta_output.cta_radialDistance_left     = -1.0f;
   cta_output.cta_objPoseX_left           = 202.0f;
   cta_output.cta_objPoseY_left           = 65.0f;
   cta_output.cta_objVelocityX_left       = 56.0f;
   cta_output.cta_objVelocityY_left       = 52.0f;
   cta_output.f_cta_alert_left            = FBK_ONE_UINT;
   cta_output.f_cta_warn_left             = FBK_ONE_UINT;
   cta_output.f_cta_prefill_req_left      = FBK_ONE_UINT;
   cta_output.f_cta_braking_req_left      = FBK_ONE_UINT;
   cta_output.f_cta_hold_supp_left        = FBK_ONE_UINT;
   cta_output.RCTA_Criticality_level_left = FBK_ONE_UINT;
   cta_output.cta_heading_rear_right      = -2.0f;

   /* obsolete error flags are beeing mocked */
   cta_output.error_flags.f_error_flag_unused   = FBK_ONE_UINT;
   cta_output.error_flags.f_cals_ptr_null       = FBK_ONE_UINT;
   cta_output.error_flags.f_input_ptr_null      = FBK_ONE_UINT;
   cta_output.error_flags.f_persistant_ptr_null = FBK_ONE_UINT;
   cta_output.error_flags.f_tracker_ptr_null    = FBK_ONE_UINT;
   cta_output.error_flags.f_vehicle_ptr_null    = FBK_ONE_UINT;


   /** \action call function. */
   Cta_Reset_Output(&cta_output);

   /** \assert Check if the variables are correctly reset. */
   EXPECT_EQ(cta_output.cta_id_right, FBK_ZERO_INT);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_intersectionX_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_radialDistance_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseX_right, 20.0024205f);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseY_right, 100.0062665f);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityX_right, 50.00641f);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityY_right, 50.00641f);
   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_prefill_req_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_braking_req_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_hold_supp_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.RCTA_Criticality_level_right, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(cta_output.cta_heading_rear_left, FBK_ZERO_F);
   EXPECT_EQ(cta_output.cta_id_left, FBK_ZERO_INT);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_intersectionX_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_radialDistance_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseX_left, 20.0024205f);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseY_left, 100.0062665f);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityX_left, 50.00641f);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityY_left, 50.00641f);
   EXPECT_EQ(cta_output.f_cta_alert_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_prefill_req_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_braking_req_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_hold_supp_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.RCTA_Criticality_level_left, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(cta_output.cta_heading_rear_right, FBK_ZERO_F);
   EXPECT_EQ(cta_output.error_flags.f_error_flag_unused, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.error_flags.f_cals_ptr_null, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.error_flags.f_input_ptr_null, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.error_flags.f_persistant_ptr_null, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.error_flags.f_tracker_ptr_null, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.error_flags.f_vehicle_ptr_null, FBK_ZERO_UINT);
}
/*
Test if the output is
                              correctly mapped from the core; \uts{CSCSA-70402} \sdd{SF-4019} \testtype{positive}
                             */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__test_rear_heading)
{
   /** \arrange Set up minimal core output */
   Cta_Reset_Output(&cta_output);
   uint8_t left_index  = FBK_ONE_UINT;
   uint8_t right_index = 2u;

   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]       = left_index;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] = CTA_CRIT_LEVEL_1;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 0.4f;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 20.0f;

   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT]       = right_index;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] = CTA_CRIT_LEVEL_1;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 1.2f;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 21.0f;

   p_vehicle_data->host_length = 5.0f;

   data.object_data[left_index].length  = 3.0f;
   data.object_data[right_index].length = 3.0f;
   data.object_data[left_index].width   = 1.5f;
   data.object_data[right_index].width  = 1.5f;

   cta_instance.core_output.cta_status = CTA_STATUS_ACTIVE;

   /** \action call function. */
   Cta_Set_Output(&cta_output, &cta_instance, p_vehicle_data);

   /** \assert Check if the variables are correctly reset. */
   EXPECT_FLOAT_EQ(cta_output.cta_heading_rear_left, cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   EXPECT_FLOAT_EQ(cta_output.cta_heading_rear_right, cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
}

/**
 * Test if the output is correctly mapped from the core;
 * \uts{CSCSA-70403} \sdd{SF-4019} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__test_no_alert)
{
   /** \arrange Set up minimal core output */
   Cta_Reset_Output(&cta_output);
   uint8_t left_index  = 1u;
   uint8_t right_index = 2u;

   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]       = left_index;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] = CTA_CRIT_LEVEL_NONE;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 0.4f;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 20.0f;

   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT]       = right_index;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] = CTA_CRIT_LEVEL_NONE;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 1.2f;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 21.0f;

   p_vehicle_data->host_length = 5.0f;

   data.object_data[left_index].length                 = 3.0f;
   data.object_data[right_index].length                = 3.0f;
   data.object_data[left_index].width                  = 1.5f;
   data.object_data[right_index].width                 = 1.5f;
   data.object_data[right_index].f_is_in_rr_sensor_fov = 1u;
   data.object_data[left_index].f_is_in_rl_sensor_fov  = 1u;

   cta_instance.core_output.cta_status = CTA_STATUS_ACTIVE;

   /** \action call function. */
   Cta_Set_Output(&cta_output, &cta_instance, p_vehicle_data);

   /** \assert Check if the variables are correctly reset. */
   EXPECT_EQ(cta_output.f_cta_enabled, 1u);

   EXPECT_EQ(cta_output.f_cta_alert_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.RCTA_Criticality_level_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_id_left, FBK_ZERO_INT);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_intersectionX_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_radialDistance_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseX_left, 20.0024205f);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseY_left, 100.0062665f);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityX_left, 50.00641f);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityY_left, 50.00641f);
   EXPECT_FLOAT_EQ(cta_output.cta_heading_rear_left, FBK_ZERO_F);

   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.RCTA_Criticality_level_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_id_right, FBK_ZERO_INT);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_intersectionX_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_radialDistance_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseX_right, 20.0024205f);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseY_right, 100.0062665f);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityX_right, 50.00641f);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityY_right, 50.00641f);
   EXPECT_FLOAT_EQ(cta_output.cta_heading_rear_right, FBK_ZERO_F);
}

/**
 * Test if the output is correctly mapped from the core;
 * \uts{CSCSA-70404} \sdd{SF-4019} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__test_warn_left)
{
   /** \arrange Set up minimal core output */
   Cta_Reset_Core_Output(&cta_instance.core_output);
   Cta_Reset_Output(&cta_output);
   uint8_t left_index  = FBK_ONE_UINT;
   uint8_t right_index = 2u;

   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]       = left_index;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] = CTA_CRIT_LEVEL_1;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 0.4f;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 20.0f;

   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT]       = right_index;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] = CTA_CRIT_LEVEL_NONE;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 1.2f;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 21.0f;

   p_vehicle_data->host_length = 5.0f;

   data.object_data[left_index].length                 = 3.0f;
   data.object_data[right_index].length                = 3.0f;
   data.object_data[left_index].width                  = 1.5f;
   data.object_data[right_index].width                 = 1.5f;
   data.object_data[right_index].f_is_in_rr_sensor_fov = 1u;
   data.object_data[left_index].f_is_in_rl_sensor_fov  = 1u;

   cta_instance.core_output.cta_status = CTA_STATUS_ACTIVE;

   /** \action call function. */
   Cta_Set_Output(&cta_output, &cta_instance, p_vehicle_data);

   /** \assert Check if the variables are correctly reset. */
   EXPECT_EQ(cta_output.f_cta_enabled, FBK_ONE_UINT);

   EXPECT_EQ(cta_output.f_cta_alert_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_left, FBK_ONE_UINT);
   EXPECT_EQ(cta_output.RCTA_Criticality_level_left, FBK_ONE_UINT);
   EXPECT_EQ(cta_output.cta_id_left, FBK_ZERO_INT);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_left, 20.0f);
   EXPECT_FLOAT_EQ(cta_output.cta_intersectionX_left, 100.0f);
   EXPECT_FLOAT_EQ(cta_output.cta_radialDistance_left, 5.0f);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseX_left, 6.6736536f);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseY_left, 0.10669249f);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityX_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityY_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_heading_rear_left, 0.40000001f);

   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.RCTA_Criticality_level_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_id_right, FBK_ZERO_INT);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_intersectionX_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_radialDistance_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseX_right, 20.0024205f);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseY_right, 100.0062665f);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityX_right, 50.00641f);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityY_right, 50.00641f);
   EXPECT_FLOAT_EQ(cta_output.cta_heading_rear_right, FBK_ZERO_F);
}

/**
 * Test if the output is correctly mapped from the core;
 * \uts{CSCSA-70405} \sdd{SF-4019} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__test_warn_right)
{
   /** \arrange Set up minimal core output */
   Cta_Reset_Output(&cta_output);
   Cta_Reset_Core_Output(&cta_instance.core_output);
   uint8_t left_index  = FBK_ONE_UINT;
   uint8_t right_index = 2u;

   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]       = left_index;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] = CTA_CRIT_LEVEL_NONE;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 0.4f;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 20.0f;

   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT]       = right_index;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] = CTA_CRIT_LEVEL_1;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 1.2f;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 21.0f;

   p_vehicle_data->host_length = 5.0f;

   data.object_data[left_index].length                 = 3.0f;
   data.object_data[right_index].length                = 3.0f;
   data.object_data[left_index].width                  = 1.5f;
   data.object_data[right_index].width                 = 1.5f;
   data.object_data[right_index].f_is_in_rr_sensor_fov = 1u;
   data.object_data[left_index].f_is_in_rl_sensor_fov  = 1u;

   cta_instance.core_output.cta_status = CTA_STATUS_ACTIVE;

   /** \action call function. */
   Cta_Set_Output(&cta_output, &cta_instance, p_vehicle_data);

   /** \assert Check if the variables are correctly reset. */
   EXPECT_EQ(cta_output.f_cta_enabled, FBK_ONE_UINT);

   EXPECT_EQ(cta_output.f_cta_alert_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.RCTA_Criticality_level_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_id_left, FBK_ZERO_INT);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_intersectionX_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_radialDistance_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseX_left, 20.0024205f);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseY_left, 100.0062665f);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityX_left, 50.00641f);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityY_left, 50.00641f);
   EXPECT_FLOAT_EQ(cta_output.cta_heading_rear_left, FBK_ZERO_F);

   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_right, FBK_ONE_UINT);
   EXPECT_EQ(cta_output.RCTA_Criticality_level_right, FBK_ONE_UINT);
   EXPECT_EQ(cta_output.cta_id_right, FBK_ZERO_INT);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_right, 21.f);
   EXPECT_FLOAT_EQ(cta_output.cta_intersectionX_right, 100.0f);
   EXPECT_FLOAT_EQ(cta_output.cta_radialDistance_right, 5.0f);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseX_right, 4.8445802f);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseY_right, -1.6698337f);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityX_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityY_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_heading_rear_right, 1.2f);
}

/**
 * Test if the output is correctly mapped from the core;
 * \uts{CSCSA-70406} \sdd{SF-4019} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__test_alert_left)
{
   /** \arrange Set up minimal core output */
   Cta_Reset_Core_Output(&cta_instance.core_output);
   Cta_Reset_Output(&cta_output);
   uint8_t left_index  = FBK_ONE_UINT;
   uint8_t right_index = 2u;

   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]       = left_index;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 0.4f;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 20.0f;

   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT]       = right_index;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] = CTA_CRIT_LEVEL_NONE;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 1.2f;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 21.0f;

   p_vehicle_data->host_length = 5.0f;

   data.object_data[left_index].length                 = 3.0f;
   data.object_data[right_index].length                = 3.0f;
   data.object_data[left_index].width                  = 1.5f;
   data.object_data[right_index].width                 = 1.5f;
   data.object_data[right_index].f_is_in_rr_sensor_fov = 1u;
   data.object_data[left_index].f_is_in_rl_sensor_fov  = 1u;

   cta_instance.core_output.cta_status = CTA_STATUS_ACTIVE;

   /** \action call function. */
   Cta_Set_Output(&cta_output, &cta_instance, p_vehicle_data);

   /** \assert Check if the variables are correctly reset. */
   EXPECT_EQ(cta_output.f_cta_enabled, FBK_ONE_UINT);

   EXPECT_EQ(cta_output.f_cta_alert_left, FBK_ONE_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_left, FBK_ONE_UINT);
}

/**
 * Test if the output is correctly mapped from the core;
 * \uts{CSCSA-70407} \sdd{SF-4019} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__test_alert_right)
{
   /** \arrange Set up minimal core output */
   Cta_Reset_Output(&cta_output);
   Cta_Reset_Core_Output(&cta_instance.core_output);
   uint8_t left_index  = FBK_ONE_UINT;
   uint8_t right_index = 2u;

   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]       = left_index;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] = CTA_CRIT_LEVEL_NONE;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 0.4f;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 20.0f;

   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT]       = right_index;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 1.2f;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 21.0f;

   p_vehicle_data->host_length = 5.0f;

   data.object_data[left_index].length                 = 3.0f;
   data.object_data[right_index].length                = 3.0f;
   data.object_data[left_index].width                  = 1.5f;
   data.object_data[right_index].width                 = 1.5f;
   data.object_data[right_index].f_is_in_rr_sensor_fov = 1u;
   data.object_data[left_index].f_is_in_rl_sensor_fov  = 1u;

   cta_instance.core_output.cta_status = CTA_STATUS_ACTIVE;

   /** \action call function. */
   Cta_Set_Output(&cta_output, &cta_instance, p_vehicle_data);

   /** \assert Check if the variables are correctly reset. */
   EXPECT_EQ(cta_output.f_cta_enabled, FBK_ONE_UINT);

   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_ONE_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_right, FBK_ONE_UINT);
}

/**
 * Test if the output is correctly mapped from the core;
 * \uts{CSCSA-70408} \sdd{SF-4019} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__test_alert_left_outside_fov)
{
   /** \arrange Set up minimal core output */
   Cta_Reset_Core_Output(&cta_instance.core_output);
   Cta_Reset_Output(&cta_output);
   uint8_t left_index  = FBK_ONE_UINT;
   uint8_t right_index = 2u;

   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]       = left_index;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 0.4f;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 20.0f;

   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT]       = right_index;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] = CTA_CRIT_LEVEL_NONE;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 1.2f;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 21.0f;

   p_vehicle_data->host_length = 5.0f;

   data.object_data[left_index].length                 = 3.0f;
   data.object_data[right_index].length                = 3.0f;
   data.object_data[left_index].width                  = 1.5f;
   data.object_data[right_index].width                 = 1.5f;
   data.object_data[right_index].f_is_in_rr_sensor_fov = 1u;
   data.object_data[left_index].f_is_in_rl_sensor_fov  = 0u;

   cta_instance.core_output.cta_status = CTA_STATUS_ACTIVE;

   /** \action call function. */
   Cta_Set_Output(&cta_output, &cta_instance, p_vehicle_data);

   /** \assert Check if the variables are correctly reset. */
   EXPECT_EQ(cta_output.f_cta_enabled, FBK_ONE_UINT);

   EXPECT_EQ(cta_output.f_cta_alert_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_left, FBK_ZERO_UINT);
}

/**
 * Test if the output is correctly mapped from the core;
 * \uts{CSCSA-70409} \sdd{SF-4019} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__test_alert_right_outside_fov)
{
   /** \arrange Set up minimal core output */
   Cta_Reset_Output(&cta_output);
   Cta_Reset_Core_Output(&cta_instance.core_output);
   uint8_t left_index  = FBK_ONE_UINT;
   uint8_t right_index = 2u;

   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]       = left_index;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] = CTA_CRIT_LEVEL_NONE;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 0.4f;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 20.0f;

   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT]       = right_index;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 1.2f;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 21.0f;

   p_vehicle_data->host_length = 5.0f;

   data.object_data[left_index].length                 = 3.0f;
   data.object_data[right_index].length                = 3.0f;
   data.object_data[left_index].width                  = 1.5f;
   data.object_data[right_index].width                 = 1.5f;
   data.object_data[right_index].f_is_in_rr_sensor_fov = 0u;
   data.object_data[left_index].f_is_in_rl_sensor_fov  = 1u;

   cta_instance.core_output.cta_status = CTA_STATUS_ACTIVE;

   /** \action call function. */
   Cta_Set_Output(&cta_output, &cta_instance, p_vehicle_data);

   /** \assert Check if the variables are correctly reset. */
   EXPECT_EQ(cta_output.f_cta_enabled, FBK_ONE_UINT);

   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_right, FBK_ZERO_UINT);
}
