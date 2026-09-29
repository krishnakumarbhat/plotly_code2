/**
 * @file recw_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SRR5 RECW post run tests
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44393}
 */

#include "recw_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "recw_post_run.c"
#include "recw_state_machine.c"
}

/**
 * Test that RECW output is filled from core output if level 1 alert is present.
 * \uts{CSCSA-44394} \sdd{SF-8005} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__is_filled_properly_for_level_1_alert)
{
   /** \arrange Set core output with level 1 alert and tracker data. */
   recw_input.c_recw_enable                     = RECW_STATE_ENABLED;
   recw_input.recw_type                         = RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH;
   recw_core_output.recw_id                     = 4u;
   recw_core_output.recw_alert_level            = RECW_ALERT_ACTIVE_LEVEL_1;
   recw_core_output.ttc_threshold_alert_level_1 = 1.0f;

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);

   /** \assert Verify that RECW output is filled correctly with tracker data and alert. */
   EXPECT_FLOAT_EQ(recw_output.recw_ttc_warning_threshold, 1.0f);
   EXPECT_EQ(recw_output.recw_obj_id, 4u);
}

/**
 * Test that RECW output is filled from core output if level 1 alert is present.
 * \uts{} \sdd{SF-8005} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__is_filled_properly_for_level_1_alert_recw_type_true)
{
   /** \arrange Set core output with level 1 alert and tracker data. */
   recw_input.c_recw_enable                     = RECW_STATE_ENABLED;
   recw_input.recw_type                         = RECW_BMW_SP25_TYPE_WARNING_ONLY;
   recw_core_output.recw_id                     = 4u;
   recw_core_output.recw_alert_level            = RECW_ALERT_ACTIVE_LEVEL_1;
   recw_core_output.ttc_threshold_alert_level_1 = 1.0f;

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);

   /** \assert Verify that RECW output is filled correctly with tracker data and alert. */
   EXPECT_FLOAT_EQ(recw_output.recw_ttc_warning_threshold, 1.0f);
   EXPECT_EQ(recw_output.recw_obj_id, 4u);
}

/**
 * Test that RECW output is filled from core output if level 1 no alert is present.
 * \uts{} \sdd{SF-8005} \testtype{negative}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__is_filled_properly_for_level_1_no_alert_recw_type_false)
{
   /** \arrange Set core output with level 1 alert and tracker data. */
   recw_input.c_recw_enable                     = RECW_STATE_ENABLED;
   recw_input.recw_type                         = RECW_BMW_SP25_TYPE_PRECRASH_ONLY;
   recw_core_output.recw_id                     = 4u;
   recw_core_output.recw_alert_level            = RECW_ALERT_ACTIVE_LEVEL_1;
   recw_core_output.ttc_threshold_alert_level_1 = 1.0f;

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);

   /** \assert Verify that RECW output is filled correctly. */
   EXPECT_NE(recw_output.recw_ttc_warning_threshold, recw_core_output.ttc_threshold_alert_level_1);
   EXPECT_NE(recw_output.recw_obj_id, recw_core_output.recw_id);
}

/**
 * Test that RECW output is filled from core output if level 2 alert is present.
 * \uts{CSCSA-44395} \sdd{SF-8005} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__is_filled_properly_for_level_2_alert)
{
   /** \arrange Set core output with level 2 alert and tracker data. */
   uint8_t obj_index                            = 3u;
   recw_core_output.recw_alert_level            = RECW_ALERT_ACTIVE_LEVEL_2;
   recw_core_output.recw_index                  = obj_index;
   recw_core_output.recw_id                     = 4u;
   recw_core_output.recw_ttc                    = 2.0f;
   recw_core_output.recw_crash_prob_combined    = 0.2f;
   recw_core_output.ttc_threshold_alert_level_1 = 1.0f;
   object_data[obj_index].speed                 = 1.0f;
   object_data[obj_index].vcs_pos.y             = 0.1f;
   object_data[obj_index].vcs_pos.x             = 10.1f;
   object_data[obj_index].obj_class             = PA_OBJ_CLASS_CAR;
   recw_input.c_recw_enable                     = RECW_STATE_ENABLED;
   recw_input.recw_type                         = RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH;

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);

   /** \assert Verify that RECW output is filled correctly with tracker data and acute alert. */
   EXPECT_EQ(recw_output.recw_obj_id, 4u);
   EXPECT_FLOAT_EQ(recw_output.recw_crash_probability, 20.0f);
   EXPECT_FLOAT_EQ(recw_output.recw_ttc_warning_threshold, 1.0f);
   EXPECT_FLOAT_EQ(recw_output.recw_obj_lat_pos, 0.1f);
   EXPECT_FLOAT_EQ(recw_output.recw_obj_long_pos, 10.1f);
   EXPECT_EQ(recw_output.recw_obj_class_cdc, RECW_BMW_SP25_OBJECT_CLASS_CDC_CAR);
}

/**
 * Test that RECW output is filled from core output if level 2 alert is present.
 * \uts{} \sdd{SF-8005} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__is_filled_properly_for_level_2_alert_recw_type)
{
   /** \arrange Set core output with level 2 alert and tracker data. */
   uint8_t obj_index                            = 3u;
   recw_core_output.recw_alert_level            = RECW_ALERT_ACTIVE_LEVEL_2;
   recw_core_output.recw_index                  = obj_index;
   recw_core_output.recw_id                     = 4u;
   recw_core_output.recw_ttc                    = 2.0f;
   recw_core_output.recw_crash_prob_combined    = 0.2f;
   recw_core_output.ttc_threshold_alert_level_1 = 1.0f;
   object_data[obj_index].speed                 = 1.0f;
   object_data[obj_index].vcs_pos.y             = 0.1f;
   object_data[obj_index].vcs_pos.x             = 10.1f;
   object_data[obj_index].obj_class             = PA_OBJ_CLASS_TRUCK;
   recw_input.c_recw_enable                     = RECW_STATE_ENABLED;
   recw_input.recw_type                         = RECW_BMW_SP25_TYPE_PRECRASH_ONLY;

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);

   /** \assert Verify that RECW output is filled correctly with tracker data and acute alert. */
   EXPECT_EQ(recw_output.recw_obj_id, 4u);
   EXPECT_FLOAT_EQ(recw_output.recw_crash_probability, 20.0f);
   EXPECT_FLOAT_EQ(recw_output.recw_ttc_warning_threshold, 1.0f);
   EXPECT_FLOAT_EQ(recw_output.recw_obj_lat_pos, 0.1f);
   EXPECT_FLOAT_EQ(recw_output.recw_obj_long_pos, 10.1f);
   EXPECT_EQ(recw_output.recw_obj_class_cdc, RECW_BMW_SP25_OBJECT_CLASS_CDC_TRUCK);
}

/**
 * Test that RECW output is filled from core output if level 2 no alert is present.
 * \uts{} \sdd{SF-8005} \testtype{negative}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__is_filled_properly_for_level_2_no_alert_recw_type_false)
{
   /** \arrange Set core output with level 1 alert and tracker data. */
   recw_input.c_recw_enable                     = RECW_STATE_ENABLED;
   recw_input.recw_type                         = RECW_BMW_SP25_TYPE_WARNING_ONLY;
   recw_core_output.recw_id                     = 4u;
   recw_core_output.recw_alert_level            = RECW_ALERT_ACTIVE_LEVEL_2;
   recw_core_output.ttc_threshold_alert_level_1 = 1.0f;

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);

   /** \assert Verify that RECW output is filled correctly. */
   EXPECT_NE(recw_output.recw_ttc_warning_threshold, recw_core_output.ttc_threshold_alert_level_1);
   EXPECT_NE(recw_output.recw_obj_id, recw_core_output.recw_id);
}


/**
 * Test the default constructor of bmw_sp25 recw output.
 * \uts{CSCSA-44396} \sdd{SF-7999} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Reset_Bmw_Sp25_Output__check_default_values)
{
   /** \arrange Set bmw srr5 recw output to non default values. */

   recw_output.recw_status                   = 1u;
   recw_output.recw_status_collision_warning = 1u;
   recw_output.recw_status_precrash          = 1u;
   recw_output.recw_obj_id                   = 1u;
   recw_output.recw_ttc                      = 1.0f;
   recw_output.recw_obj_distance             = 1.0f;
   recw_output.recw_obj_approach_speed       = 1.0f;
   recw_output.recw_obj_lat_pos              = 1.0f;
   recw_output.recw_obj_long_pos             = 1.0f;
   recw_output.recw_obj_heading              = 1.0f;
   recw_output.recw_crash_probability        = 1.0f;
   recw_output.recw_overlap                  = 1.0f;
   recw_output.recw_ttc_warning_threshold    = 1.0f;
   recw_output.recw_obj_class                = FBK_ZERO_UINT;
   recw_output.recw_obj_class_cdc            = (uint8_t) RECW_BMW_SP25_OBJECT_CLASS_CDC_TRUCK;


   /** \action Call Recw_Update_Recw_Output to update output of RECW. */
   Recw_Reset_Bmw_Sp25_Output(&recw_output);

   /** \assert Check that default values are applied. */
   EXPECT_EQ(recw_output.recw_status, 0u);
   EXPECT_EQ(recw_output.recw_status_collision_warning, 0u);
   EXPECT_EQ(recw_output.recw_status_precrash, 0u);
   EXPECT_EQ(recw_output.recw_obj_id, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(recw_output.recw_ttc, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(recw_output.recw_obj_distance, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(recw_output.recw_obj_approach_speed, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(recw_output.recw_obj_lat_pos, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(recw_output.recw_obj_long_pos, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(recw_output.recw_obj_heading, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(recw_output.recw_crash_probability, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(recw_output.recw_overlap, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(recw_output.recw_ttc_warning_threshold, FBK_ZERO_F);
   EXPECT_EQ(recw_output.recw_obj_class, FBK_ZERO_UINT);
   EXPECT_EQ(recw_output.recw_obj_class_cdc, (uint8_t) RECW_BMW_SP25_OBJECT_CLASS_CDC_UNKNOWN);
   EXPECT_EQ(recw_output.recw_sm_state, RECW_SM_NOT_AVAILABLE);
}


/**
 * Test the overlap calculation function of RECW bmw srr5 post run. Here no overlap is expected, target on the left side.
 * \uts{CSCSA-44397} \sdd{SF-8004} \testtype{negative}
 */
TEST_F(Recw_Post_Run_Test, Recw_Calculate_Collision_Overlap__no_overlap_expected_left)
{
   /** \arrange Set inputs such that no overlap is given. */
   float32_T return_overlap;
   uint8_t obj_index                = 1u;
   p_vehicle_data->host_width       = 2.0f;
   object_data[obj_index].width     = 1.0f;
   object_data[obj_index].vcs_pos.y = -p_vehicle_data->host_width - object_data[obj_index].width;

   /** \action Call Overlap calculation. */
   return_overlap = Recw_Calculate_Collision_Overlap(&data, obj_index);

   /** \assert Check that an overlap of 0 is returned. */
   EXPECT_FLOAT_EQ(return_overlap, 0.0f);
}

/**
 * Test the overlap calculation function of RECW bmw srr5 post run. Here no overlap is expected, target on the right side.
 * \uts{CSCSA-44398} \sdd{SF-8004} \testtype{negative}
 */
TEST_F(Recw_Post_Run_Test, Recw_Calculate_Collision_Overlap__no_overlap_expected_right)
{
   /** \arrange Set inputs such that no overlap is given. */
   float32_T return_overlap;
   uint8_t obj_index                = 1u;
   p_vehicle_data->host_width       = 2.0f;
   object_data[obj_index].width     = 1.0f;
   object_data[obj_index].vcs_pos.y = p_vehicle_data->host_width + object_data[obj_index].width;

   /** \action Call Overlap calculation. */
   return_overlap = Recw_Calculate_Collision_Overlap(&data, obj_index);

   /** \assert Check that an overlap of 0 is returned. */
   EXPECT_FLOAT_EQ(return_overlap, 0.0f);
}

/**
 * Test the overlap calculation function of RECW bmw srr5 post run. Here Target overlapping with ego from left ego border is
 * expected. \uts{CSCSA-44399} \sdd{SF-8004} \testtype{negative}
 */
TEST_F(Recw_Post_Run_Test, Recw_Calculate_Collision_Overlap___overlap_from_left_border)
{
   /** \arrange Set inputs such that overlap from left border is given. */
   float32_T return_overlap;
   float32_T expected_overlap;
   uint8_t obj_index                     = 1u;
   data.vehicle_data.host_width          = 2.0f;
   data.object_data[obj_index].width     = 1.0f;
   data.object_data[obj_index].vcs_pos.y = -0.5f * data.vehicle_data.host_width;
   expected_overlap = Fbk_Abs_F((0.5f * data.object_data[obj_index].width) / data.object_data[obj_index].width);

   /** \action Call Overlap calculation. */
   return_overlap = Recw_Calculate_Collision_Overlap(&data, obj_index);

   /** \assert Check that an overlap is returned. */
   EXPECT_FLOAT_EQ(return_overlap, expected_overlap);
}


/**
 * Test the overlap calculation function of RECW bmw srr5 post run. Here Target overlapping with ego from right ego border is
 * expected. \uts{CSCSA-44400} \sdd{SF-8004} \testtype{negative}
 */
TEST_F(Recw_Post_Run_Test, Recw_Calculate_Collision_Overlap___overlap_from_right_border)
{
   /** \arrange Set inputs such that overlap from right border is given. */
   float32_T return_overlap;
   float32_T expected_overlap;
   uint8_t obj_index                = 1u;
   p_vehicle_data->host_width       = 2.0f;
   object_data[obj_index].width     = 1.0f;
   object_data[obj_index].vcs_pos.y = 0.5f * p_vehicle_data->host_width;
   expected_overlap                 = Fbk_Abs_F((0.5f * object_data[obj_index].width) / object_data[obj_index].width);

   /** \action Call Overlap calculation. */
   return_overlap = Recw_Calculate_Collision_Overlap(&data, obj_index);

   /** \assert Check that an overlap is returned. */
   EXPECT_FLOAT_EQ(return_overlap, expected_overlap);
}

/**
 * Test the overlap calculation function of RECW bmw srr5 post run. Here total Target overlapping with ego is expected.
 * \uts{CSCSA-44401} \sdd{SF-8004} \testtype{negative}
 */
TEST_F(Recw_Post_Run_Test, Recw_Calculate_Collision_Overlap___overlap_total)
{
   /** \arrange Set inputs such that total overlap is given. */
   float32_T return_overlap;
   float32_T expected_overlap;
   uint8_t obj_index                = 1u;
   p_vehicle_data->host_width       = 2.0f;
   object_data[obj_index].width     = 2.0f;
   object_data[obj_index].vcs_pos.y = FBK_ZERO_F;
   expected_overlap                 = 1.0f;

   /** \action Call Overlap calculation. */
   return_overlap = Recw_Calculate_Collision_Overlap(&data, obj_index);

   /** \assert Check that an overlap is returned. */
   EXPECT_FLOAT_EQ(return_overlap, expected_overlap);
}

/**
 * Test the object class cdc mapping function of Recw. Here 2wheels shall be mapped on motorcycle
 * \uts{CSCSA-44406} \sdd{SF-8002} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Map_Object_Class_Cdc__2wheels_to_motorcycle)
{
   /** \arrange Set input object class to 2wheel. */
   Pa_Obj_Class_T obj_class = PA_OBJ_CLASS_2WHEEL;
   float32_T obj_speed      = 1.1f * RECW_BMW_SP25_CDC_MAPPING_VELOCITY_THRESHOLD;
   Recw_Bmw_Sp25_Object_Class_Cdc_T res;

   /** \action Call cdc object class mapping function. */
   res = Recw_Map_Object_Class_Cdc(obj_class, obj_speed);

   /** \assert Expect motorcycle object class to be returned. */
   EXPECT_EQ(res, RECW_BMW_SP25_OBJECT_CLASS_CDC_MOTORCYCLE);
}

/**
 * Test the object class cdc mapping function of Recw. Here 2wheels shall be mapped on bicycle
 * \uts{CSCSA-44407} \sdd{SF-8002} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Map_Object_Class_Cdc__2wheels_to_bicycle)
{
   /** \arrange Set input object class to 2wheel. */
   Pa_Obj_Class_T obj_class = PA_OBJ_CLASS_2WHEEL;
   float32_T obj_speed      = 0.9f * RECW_BMW_SP25_CDC_MAPPING_VELOCITY_THRESHOLD;
   Recw_Bmw_Sp25_Object_Class_Cdc_T res;

   /** \action Call cdc object class mapping function. */
   res = Recw_Map_Object_Class_Cdc(obj_class, obj_speed);

   /** \assert Expect bicycle object class to be returned. */
   EXPECT_EQ(res, RECW_BMW_SP25_OBJECT_CLASS_CDC_BICYCLE);
}

/**
 * Test the object class cdc mapping function of Recw. Here unknown shall be mapped on unknown
 * \uts{CSCSA-44408} \sdd{SF-8002} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Map_Object_Class_Cdc__unknown_to_unknown)
{
   /** \arrange Set input object class to unknown. */
   Pa_Obj_Class_T obj_class = PA_OBJ_CLASS_UNKNOWN;
   float32_T obj_speed      = 1.1f * RECW_BMW_SP25_CDC_MAPPING_VELOCITY_THRESHOLD;
   Recw_Bmw_Sp25_Object_Class_Cdc_T res;

   /** \action Call cdc object class mapping function. */
   res = Recw_Map_Object_Class_Cdc(obj_class, obj_speed);

   /** \assert Expect unknown object class to be returned. */
   EXPECT_EQ(res, RECW_BMW_SP25_OBJECT_CLASS_CDC_UNKNOWN);
}

/**
 * Test the object class cdc mapping function of Recw. Here pedestrian shall be mapped on pedestrian
 * \uts{CSCSA-44409} \sdd{SF-8002} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Map_Object_Class_Cdc__pedestrian_to_pedestrian)
{
   /** \arrange Set input object class to pedestrian. */
   Pa_Obj_Class_T obj_class = PA_OBJ_CLASS_PEDESTRIAN;
   float32_T obj_speed      = 1.1f * RECW_BMW_SP25_CDC_MAPPING_VELOCITY_THRESHOLD;
   Recw_Bmw_Sp25_Object_Class_Cdc_T res;

   /** \action Call cdc object class mapping function. */
   res = Recw_Map_Object_Class_Cdc(obj_class, obj_speed);

   /** \assert Expect pedestrian object class to be returned. */
   EXPECT_EQ(res, RECW_BMW_SP25_OBJECT_CLASS_CDC_PEDESTRIAN);
}

/**
 * Test the object class cdc mapping function of Recw. Here default shall be mapped on unknown
 * \uts{CSCSA-44410} \sdd{SF-8002} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Map_Object_Class_Cdc__default_to_unknown)
{
   /** \arrange Set input object class to default. */
   uint8_t obj_class   = 255u;
   float32_T obj_speed = 1.1f * RECW_BMW_SP25_CDC_MAPPING_VELOCITY_THRESHOLD;
   Recw_Bmw_Sp25_Object_Class_Cdc_T res;

   /** \action Call cdc object class mapping function. */
   res = Recw_Map_Object_Class_Cdc((Pa_Obj_Class_T) obj_class, obj_speed);

   /** \assert Expect unknown object class to be returned. */
   EXPECT_EQ(res, RECW_BMW_SP25_OBJECT_CLASS_CDC_UNKNOWN);
}

/**
 * Test that RECW output is filled correctly for not available state.
 * \uts{} \sdd{SF-8005} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__StateMachine_State_Not_Available)
{
   /** \arrange recw state with not available state. */
   Recw_Reset_Bmw_Sp25_Output(&recw_output);
   recw_input.c_recw_enable = RECW_STATE_ENABLED;
   recw_input.recw_type     = RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH;
   Recw_Set_State(RECW_SM_NOT_AVAILABLE);

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);
   /** \assert Verify that RECW output is filled correctly. */
   EXPECT_EQ(recw_output.recw_status_collision_warning, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_status_precrash, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_ttc, RECW_BMW_SP25_SM_HEX_FC);
   EXPECT_EQ(recw_output.recw_obj_approach_speed, RECW_BMW_SP25_SM_HEX_FC);
   EXPECT_EQ(recw_output.recw_obj_distance, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_overlap, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_class, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_heading, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.edr_drasy_event_ID62_status_collision_warning_side_radar_rear, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.edr_drasy_event_ID62_status_pre_crash_side_radar_rear, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_sm_state, RECW_SM_NOT_AVAILABLE);
}

/**
 * Test that RECW output is filled correctly for ready state.
 * \uts{} \sdd{SF-8005} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__StateMachine_State_Ready)
{
   /** \arrange recw state with ready state. */
   Recw_Reset_Bmw_Sp25_Output(&recw_output);
   recw_input.c_recw_enable = RECW_STATE_ENABLED;
   recw_input.recw_type     = RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH;
   Recw_Set_State(RECW_SM_READY);

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);
   /** \assert Verify that RECW output is filled correctly. */
   EXPECT_EQ(recw_output.recw_status_collision_warning, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_status_precrash, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_ttc, RECW_BMW_SP25_SM_HEX_FC);
   EXPECT_EQ(recw_output.recw_obj_approach_speed, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_distance, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_overlap, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_class, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_heading, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.edr_drasy_event_ID62_status_collision_warning_side_radar_rear, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.edr_drasy_event_ID62_status_pre_crash_side_radar_rear, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_sm_state, RECW_SM_READY);
}

/**
 * Test that RECW output is filled correctly for degraded state.
 * \uts{} \sdd{SF-8005} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__StateMachine_State_Degraded)
{
   /** \arrange Set core output with degraded state. */
   Recw_Reset_Bmw_Sp25_Output(&recw_output);
   recw_input.c_recw_enable = RECW_STATE_ENABLED;
   recw_input.recw_type     = RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH;
   Recw_Set_State(RECW_SM_DEGRADED);

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);

   /** \assert Verify that RECW output is filled correctly with tracker data and alert. */
   EXPECT_EQ(recw_output.recw_status_collision_warning, RECW_BMW_SP25_SM_HEX_6);
   EXPECT_EQ(recw_output.recw_status_precrash, RECW_BMW_SP25_SM_HEX_E);
   EXPECT_EQ(recw_output.recw_ttc, RECW_BMW_SP25_SM_HEX_FE);
   EXPECT_EQ(recw_output.recw_obj_approach_speed, RECW_BMW_SP25_SM_HEX_FE);
   EXPECT_EQ(recw_output.recw_obj_distance, RECW_BMW_SP25_SM_HEX_FE);
   EXPECT_EQ(recw_output.recw_overlap, RECW_BMW_SP25_SM_HEX_E);
   EXPECT_EQ(recw_output.recw_obj_class, RECW_BMW_SP25_SM_HEX_E);
   EXPECT_EQ(recw_output.recw_obj_heading, RECW_BMW_SP25_SM_HEX_FE);
   EXPECT_EQ(recw_output.edr_drasy_event_ID62_status_collision_warning_side_radar_rear, RECW_BMW_SP25_SM_HEX_6);
   EXPECT_EQ(recw_output.edr_drasy_event_ID62_status_pre_crash_side_radar_rear, RECW_BMW_SP25_SM_HEX_E);
   EXPECT_EQ(recw_output.recw_sm_state, RECW_SM_DEGRADED);
}

/**
 * Test that RECW output is filled correctly for error state.
 * \uts{} \sdd{SF-8005} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__StateMachine_State_Error)
{
   /** \arrange Set core output with error state. */
   Recw_Reset_Bmw_Sp25_Output(&recw_output);
   recw_input.c_recw_enable = RECW_STATE_ENABLED;
   recw_input.recw_type     = RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH;
   Recw_Set_State(RECW_SM_ERROR);

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);

   /** \assert Verify that RECW output is filled correctly with tracker data and alert. */
   EXPECT_EQ(recw_output.recw_status_collision_warning, RECW_BMW_SP25_SM_HEX_6);
   EXPECT_EQ(recw_output.recw_status_precrash, RECW_BMW_SP25_SM_HEX_E);
   EXPECT_EQ(recw_output.recw_ttc, RECW_BMW_SP25_SM_HEX_FE);
   EXPECT_EQ(recw_output.recw_obj_approach_speed, RECW_BMW_SP25_SM_HEX_FE);
   EXPECT_EQ(recw_output.recw_obj_distance, RECW_BMW_SP25_SM_HEX_FE);
   EXPECT_EQ(recw_output.recw_overlap, RECW_BMW_SP25_SM_HEX_E);
   EXPECT_EQ(recw_output.recw_obj_class, RECW_BMW_SP25_SM_HEX_E);
   EXPECT_EQ(recw_output.recw_obj_heading, RECW_BMW_SP25_SM_HEX_FE);
   EXPECT_EQ(recw_output.edr_drasy_event_ID62_status_collision_warning_side_radar_rear, RECW_BMW_SP25_SM_HEX_6);
   EXPECT_EQ(recw_output.edr_drasy_event_ID62_status_pre_crash_side_radar_rear, RECW_BMW_SP25_SM_HEX_E);
   EXPECT_EQ(recw_output.recw_sm_state, RECW_SM_ERROR);
}

/**
 * Test that RECW output is filled correctly for undefined state.
 * \uts{} \sdd{SF-8005} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__Undefined_StateMachine_State)
{
   /** \arrange Set core output with error state. */
   Recw_Reset_Bmw_Sp25_Output(&recw_output);
   recw_input.c_recw_enable = RECW_STATE_ENABLED;
   recw_input.recw_type     = RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH;
   Recw_Set_State((Recw_SM_State_T) 5);

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);

   /** \assert Verify that RECW output is filled correctly with tracker data and alert. */
   EXPECT_EQ(recw_output.recw_status_collision_warning, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_status_precrash, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_ttc, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_approach_speed, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_distance, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_overlap, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_class, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_heading, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.edr_drasy_event_ID62_status_collision_warning_side_radar_rear, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.edr_drasy_event_ID62_status_pre_crash_side_radar_rear, RECW_BMW_SP25_SM_HEX_0);
}

/**
 * Test that RECW output is filled from core output if level 1 alert is present.
 * \uts{} \sdd{SF-8005} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__StateMachine_State_Active_Level_1_True)
{
   /** \arrange Set core output with level 1 alert and tracker data. */
   Recw_Reset_Bmw_Sp25_Output(&recw_output);
   recw_input.c_recw_enable          = RECW_STATE_ENABLED;
   recw_input.recw_type              = RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH;
   recw_core_output.recw_alert_level = RECW_ALERT_ACTIVE_LEVEL_1;
   Recw_Set_State(RECW_SM_ACTIVE);

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);

   /** \assert Verify that RECW output is filled correctly with tracker data and alert. */
   EXPECT_EQ(recw_output.recw_status_collision_warning, RECW_BMW_SP25_SM_HEX_1);
   EXPECT_EQ(recw_output.recw_sm_state, RECW_SM_ACTIVE);
}

/**
 * Test that RECW output is filled from core output if level 1 alert is present.
 * \uts{} \sdd{SF-8005} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__StateMachine_State_Active_Level_1_True_recw_type_warning)
{
   /** \arrange Set core output with level 1 alert and tracker data. */
   Recw_Reset_Bmw_Sp25_Output(&recw_output);
   recw_input.c_recw_enable          = RECW_STATE_ENABLED;
   recw_input.recw_type              = RECW_BMW_SP25_TYPE_WARNING_ONLY;
   recw_core_output.recw_alert_level = RECW_ALERT_ACTIVE_LEVEL_2;
   Recw_Set_State(RECW_SM_ACTIVE);

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);

   /** \assert Verify that RECW output is filled correctly. */
   EXPECT_EQ(recw_output.recw_obj_approach_speed, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_ttc, RECW_BMW_SP25_SM_HEX_FC);
   EXPECT_EQ(recw_output.recw_status_precrash, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.edr_drasy_event_ID62_status_pre_crash_side_radar_rear, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_distance, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_overlap, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_class, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_heading, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_sm_state, RECW_SM_ACTIVE);
}

/**
 * Test that RECW output is filled from core output if level 1 alert is present.
 * \uts{} \sdd{SF-8005} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__StateMachine_State_Active_Level_1_True_recw_type_precrash)
{
   /** \arrange Set core output with level 1 alert and tracker data. */
   Recw_Reset_Bmw_Sp25_Output(&recw_output);
   recw_input.c_recw_enable          = RECW_STATE_ENABLED;
   recw_input.recw_type              = RECW_BMW_SP25_TYPE_PRECRASH_ONLY;
   recw_core_output.recw_index       = 1u;
   p_vehicle_data->host_speed        = 5.0f;
   object_data[1].speed              = 1.0f;
   recw_core_output.recw_ttc         = 3.0f;
   object_data[1].vcs_pos.x          = 5.0f;
   object_data[1].vcs_pos.y          = 1.0f;
   p_vehicle_data->host_width        = 2.0f;
   object_data[1].width              = 2.0f;
   object_data[1].obj_class          = PA_OBJ_CLASS_TRUCK;
   object_data[1].vcs_heading        = 2.0f;
   recw_core_output.recw_alert_level = RECW_ALERT_ACTIVE_LEVEL_2;
   Recw_Set_State(RECW_SM_ACTIVE);

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);

   /** \assert Verify that RECW output is filled correctly. */
   EXPECT_EQ(recw_output.recw_ttc, recw_core_output.recw_ttc);
   EXPECT_EQ(recw_output.recw_status_precrash, RECW_BMW_SP25_SM_HEX_4);
   EXPECT_EQ(recw_output.edr_drasy_event_ID62_status_pre_crash_side_radar_rear, RECW_BMW_SP25_SM_HEX_4);
}

/**
 * Test that RECW output is filled from core output if level 1 alert is present.
 * \uts{} \sdd{SF-8005} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__StateMachine_State_Active_Level_1_False)
{
   /** \arrange Set core output with active state and level 1 warning false. */
   Recw_Reset_Bmw_Sp25_Output(&recw_output);
   recw_input.c_recw_enable          = RECW_STATE_ENABLED;
   recw_input.recw_type              = RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH;
   recw_core_output.recw_alert_level = RECW_NO_ALERT;
   Recw_Set_State(RECW_SM_ACTIVE);

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);
   /** \assert Verify that RECW output is filled correctly. */
   EXPECT_EQ(recw_output.recw_status_collision_warning, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_sm_state, RECW_SM_ACTIVE);
}

/**
 * Test that RECW output is filled correctly for active state level 2 warning.
 * \uts{} \sdd{SF-8005} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__StateMachine_State_Active_Level_2_True)
{
   /** \arrange Set core output with active state and level 2 warning true. */
   uint8_t obj_index = 1u;
   Recw_Reset_Bmw_Sp25_Output(&recw_output);
   recw_input.c_recw_enable          = RECW_STATE_ENABLED;
   recw_input.recw_type              = RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH;
   recw_core_output.recw_alert_level = RECW_ALERT_ACTIVE_LEVEL_2;
   Recw_Set_State(RECW_SM_ACTIVE);
   recw_core_output.recw_index        = obj_index;
   p_vehicle_data->host_speed         = 5.0f;
   object_data[obj_index].speed       = 1.0f;
   recw_core_output.recw_ttc          = 3.0f;
   object_data[obj_index].vcs_pos.x   = 5.0f;
   object_data[obj_index].vcs_pos.y   = 1.0f;
   p_vehicle_data->host_width         = 2.0f;
   object_data[obj_index].width       = 2.0f;
   object_data[obj_index].obj_class   = PA_OBJ_CLASS_TRUCK;
   object_data[obj_index].vcs_heading = 2.0f;

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);
   /** \assert Verify that RECW output is filled correctly. */
   EXPECT_EQ(recw_output.recw_status_collision_warning, RECW_BMW_SP25_SM_HEX_4);
   EXPECT_EQ(recw_output.recw_status_precrash, RECW_BMW_SP25_SM_HEX_4);
   EXPECT_FLOAT_EQ(recw_output.recw_ttc, 3.0f);
   EXPECT_FLOAT_EQ(recw_output.recw_obj_approach_speed, 4.0f);
   EXPECT_NEAR(recw_output.recw_obj_distance, 5.099f, 0.001f);
   EXPECT_FLOAT_EQ(recw_output.recw_overlap, 50.0f);
   EXPECT_EQ(recw_output.recw_obj_class, 4u);
   EXPECT_FLOAT_EQ(recw_output.recw_obj_heading, 2.0f);
   EXPECT_EQ(recw_output.recw_sm_state, RECW_SM_ACTIVE);
}

/**
 * Test that RECW output is filled correctly for active state no level 2 warning.
 * \uts{} \sdd{SF-8005} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__StateMachine_State_Active_Level_2_False)
{
   /** \arrange Set core output with active state and level 2 warning false. */
   Recw_Reset_Bmw_Sp25_Output(&recw_output);
   recw_input.c_recw_enable          = RECW_STATE_ENABLED;
   recw_input.recw_type              = RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH;
   recw_core_output.recw_alert_level = RECW_ALERT_ACTIVE_LEVEL_1;
   Recw_Set_State(RECW_SM_ACTIVE);

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);

   /** \assert Verify that RECW output is filled correctly. */
   EXPECT_EQ(recw_output.recw_status_precrash, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_ttc, RECW_BMW_SP25_SM_HEX_FC);
   EXPECT_EQ(recw_output.recw_obj_approach_speed, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_distance, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_overlap, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_class, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_heading, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.edr_drasy_event_ID62_status_pre_crash_side_radar_rear, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_sm_state, RECW_SM_ACTIVE);
}

/**
 * Test that RECW output is filled from core output if level 2 alert is present.
 * \uts{} \sdd{SF-8005} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__StateMachine_State_Active_Level_2_False_recw_type)
{
   /** \arrange Set core output with active state and level 2 warning false recw type. */
   Recw_Reset_Bmw_Sp25_Output(&recw_output);
   recw_input.c_recw_enable          = RECW_STATE_ENABLED;
   recw_input.recw_type              = RECW_BMW_SP25_TYPE_PRECRASH_ONLY;
   recw_core_output.recw_alert_level = RECW_ALERT_ACTIVE_LEVEL_1;
   Recw_Set_State(RECW_SM_ACTIVE);

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);

   /** \assert Verify that RECW output is filled correctly */
   EXPECT_EQ(recw_output.recw_status_precrash, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_ttc, RECW_BMW_SP25_SM_HEX_FC);
   EXPECT_EQ(recw_output.recw_obj_approach_speed, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_distance, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_overlap, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_class, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_heading, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.edr_drasy_event_ID62_status_pre_crash_side_radar_rear, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_sm_state, RECW_SM_ACTIVE);
}

/**
 * Test that RECW output is filled from core output if level 1 alert is present.
 * \uts{} \sdd{SF-8005} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__StateMachine_State_Active_Level_1_True_recw_type_no_function)
{
   /** \arrange Set output data with level 1 alert and recw type as no function. */
   Recw_Reset_Bmw_Sp25_Output(&recw_output);
   recw_input.c_recw_enable          = RECW_STATE_ENABLED;
   recw_input.recw_type              = RECW_BMW_SP25_TYPE_NO_FUNCTION;
   recw_core_output.recw_index       = 1u;
   p_vehicle_data->host_speed        = 5.0f;
   object_data[1].speed              = 1.0f;
   recw_core_output.recw_ttc         = 3.0f;
   object_data[1].vcs_pos.x          = 5.0f;
   object_data[1].vcs_pos.y          = 1.0f;
   p_vehicle_data->host_width        = 2.0f;
   object_data[1].width              = 2.0f;
   object_data[1].obj_class          = PA_OBJ_CLASS_TRUCK;
   object_data[1].vcs_heading        = 2.0f;
   recw_core_output.recw_alert_level = RECW_ALERT_ACTIVE_LEVEL_1;
   Recw_Set_State(RECW_SM_ACTIVE);

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);

   /** \assert Verify that RECW output is filled correctly. */
   EXPECT_EQ(recw_output.recw_status_collision_warning, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_status_precrash, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_ttc, RECW_BMW_SP25_SM_HEX_FC);
   EXPECT_EQ(recw_output.recw_obj_approach_speed, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_distance, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_overlap, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_class, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_heading, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.edr_drasy_event_ID62_status_pre_crash_side_radar_rear, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_sm_state, RECW_SM_ACTIVE);
}

/**
 * Test that RECW output is filled from core output if no alert.
 * \uts{} \sdd{SF-8005} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Update_Bmw_Sp25_Output__StateMachine_State_Active_Level_1_True_recw_type_alert_level)
{
   /** \arrange Set output data with level 1 alert and recw type as no function. */
   Recw_Reset_Bmw_Sp25_Output(&recw_output);
   recw_input.c_recw_enable          = RECW_STATE_ENABLED;
   recw_input.recw_type              = RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH;
   recw_core_output.recw_id          = 4u;
   recw_core_output.recw_alert_level = RECW_NO_ALERT;
   recw_core_output.recw_index       = 1u;
   p_vehicle_data->host_speed        = 5.0f;
   object_data[1].speed              = 1.0f;
   recw_core_output.recw_ttc         = 3.0f;
   object_data[1].vcs_pos.x          = 5.0f;
   object_data[1].vcs_pos.y          = 1.0f;
   p_vehicle_data->host_width        = 2.0f;
   object_data[1].width              = 2.0f;
   object_data[1].obj_class          = PA_OBJ_CLASS_TRUCK;
   object_data[1].vcs_heading        = 2.0f;
   Recw_Set_State(RECW_SM_ACTIVE);

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   Recw_Update_Bmw_Sp25_Output(&recw_output, &recw_input, &recw_core_output, &data);

   /** \assert Verify that RECW output is filled correctly. */
   EXPECT_EQ(recw_output.recw_status_collision_warning, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_status_precrash, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_ttc, RECW_BMW_SP25_SM_HEX_FC);
   EXPECT_EQ(recw_output.recw_obj_approach_speed, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_distance, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_overlap, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_class, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_obj_heading, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.edr_drasy_event_ID62_status_pre_crash_side_radar_rear, RECW_BMW_SP25_SM_HEX_0);
   EXPECT_EQ(recw_output.recw_sm_state, RECW_SM_ACTIVE);
}

/**
 * Test check collision overlap .
 * \uts{} \sdd{WI-19468} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Calculate_Collision_Overlap__check_overlap)
{
   /** \arrange Set output data with level 1 alert and recw type as no function. */
   float32_T overlap;
   uint8_t obj_index                            = 3u;
   recw_core_output.recw_alert_level            = RECW_ALERT_ACTIVE_LEVEL_2;
   recw_core_output.recw_index                  = obj_index;
   recw_core_output.recw_id                     = 4u;
   recw_core_output.recw_ttc                    = 2.0f;
   recw_core_output.recw_crash_prob_combined    = 0.2f;
   recw_core_output.ttc_threshold_alert_level_1 = 1.0f;
   object_data[obj_index].speed                 = 1.0f;
   object_data[obj_index].vcs_pos.y             = 0.3f;
   object_data[obj_index].vcs_pos.x             = 10.1f;
   object_data[obj_index].obj_class             = PA_OBJ_CLASS_CAR;
   object_data[obj_index].width                 = 3.5f;
   recw_input.c_recw_enable                     = RECW_STATE_ENABLED;
   recw_input.recw_type                         = RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH;
   data.vehicle_data.host_width                 = 5.0f;
   Recw_Set_State(RECW_SM_ACTIVE);

   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   overlap = Recw_Calculate_Collision_Overlap(&data, obj_index);

   /** \assert Verify that RECW output is filled correctly. */
   EXPECT_FLOAT_EQ(overlap, 1.0f);
}

/**
 * Test check collision overlap .
 * \uts{} \sdd{WI-19468} \testtype{positive}
 */
TEST_F(Recw_Post_Run_Test, Recw_Calculate_Collision_Overlap__left_border)
{
   /** \arrange Set output data with level 1 alert and recw type as no function. */
   float32_T overlap;
   uint8_t obj_index                            = 3u;
   recw_core_output.recw_alert_level            = RECW_ALERT_ACTIVE_LEVEL_2;
   recw_core_output.recw_index                  = obj_index;
   recw_core_output.recw_id                     = 4u;
   recw_core_output.recw_ttc                    = 2.0f;
   recw_core_output.recw_crash_prob_combined    = 0.2f;
   recw_core_output.ttc_threshold_alert_level_1 = 1.0f;
   object_data[obj_index].speed                 = 1.0f;
   object_data[obj_index].vcs_pos.y             = 0.1f;
   object_data[obj_index].vcs_pos.x             = 10.1f;
   object_data[obj_index].obj_class             = PA_OBJ_CLASS_CAR;
   object_data[obj_index].width                 = 3.5f;
   recw_input.c_recw_enable                     = RECW_STATE_ENABLED;
   recw_input.recw_type                         = RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH;
   data.vehicle_data.host_width                 = -3.5f;
   Recw_Set_State(RECW_SM_ACTIVE);
   float32_T obj_width        = object_data[obj_index].width;
   float32_T ego_left_border  = -0.5f * data.vehicle_data.host_width;
   float32_T obj_right_border = object_data[obj_index].vcs_pos.y + (0.5f * obj_width);


   /** \action Call Recw_Update_Bmw_Sp25_Output to update output of RECW. */
   overlap = Recw_Calculate_Collision_Overlap(&data, obj_index);

   /** \assert Verify that RECW output is filled correctly. */
   EXPECT_FLOAT_EQ(overlap, Fbk_Abs_F((obj_right_border - ego_left_border) / obj_width));
}