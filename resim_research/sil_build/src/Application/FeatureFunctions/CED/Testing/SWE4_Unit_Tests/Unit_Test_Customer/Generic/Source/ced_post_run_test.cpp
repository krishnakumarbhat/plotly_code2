/**
 * @file ced_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Generic CED post run
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-126141}
 */

#include "ced_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ced_generic_types.h"
#include "ced_post_run.c"
#include "pa_shared_types.h"

#include "fbk_macros.h"
}


/**
 * Check that post run initialization routine resets the generic output accordingly.
 * \uts{CSCSA-126142} \sdd{SF-3472} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Post_Run_Test, Ced_Post_Run_Init__check_no_fatal_failure)
{
   /** \arrange set output to non default */
   /** \action Run function to test */
   /** \assert Verify output is set to default */
   EXPECT_NO_FATAL_FAILURE(Ced_Post_Run_Init(&ced_instance););
}

/**
 * Verify if copying variables from core output and tracker is valid.
 * \uts{CSCSA-126143} \sdd{SF-3397} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Post_Run_Test, Ced_Post_Run__math_from_core_and_tracker)
{
   uint8_t obj_index = 9u;
   uint8_t obj_id    = 13u;

   /** \arrange set core output to non default */
   ced_input.f_ced_enable = FBK_TRUE;

   ced_core_output.ced_id[FBK_SIDE_RIGHT]        = obj_id;
   ced_core_output.ced_unique_id[FBK_SIDE_RIGHT] = obj_id;
   ced_core_output.ced_index[FBK_SIDE_RIGHT]     = obj_index;
   ced_core_output.ced_alert[FBK_SIDE_RIGHT]     = CED_ALERT_ACTIVE_LEVEL_2;

   ced_core_output.ced_object_direction[FBK_SIDE_RIGHT]         = FBK_SIDE_REAR;
   ced_core_output.ced_object_predicted_lat_pos[FBK_SIDE_RIGHT] = 1.7f;
   ced_core_output.ced_ttc[FBK_SIDE_RIGHT]                      = 1.8f;
   ced_core_output.ced_ttp[FBK_SIDE_RIGHT]                      = 1.9f;

   /* set tracker object parameters to non default */
   object_data[obj_index].obj_class   = PA_OBJ_CLASS_CAR;
   object_data[obj_index].length      = 1.1f;
   object_data[obj_index].width       = 1.2f;
   object_data[obj_index].vcs_pos.x   = 1.3f;
   object_data[obj_index].vcs_pos.y   = 1.4f;
   object_data[obj_index].speed       = 1.5f;
   object_data[obj_index].vcs_heading = 1.6f;

   /** \action Run Ced_Post_Run for mapping wit core output and tracker data. */
   Ced_Post_Run(&ced_instance, &ced_input, &ced_output);

   /** \assert Verify generic output is mapped to core output and tracker data correctly */
   EXPECT_EQ(ced_output.ced_alert[FBK_SIDE_RIGHT], CED_ALERT_ACTIVE_LEVEL_2);
   EXPECT_EQ(ced_output.ced_object[FBK_SIDE_RIGHT].id, obj_id);
   EXPECT_EQ(ced_output.ced_object[FBK_SIDE_RIGHT].unique_id, obj_id);
   EXPECT_EQ(ced_output.ced_object[FBK_SIDE_RIGHT].type, PA_OBJ_CLASS_CAR);
   EXPECT_FLOAT_EQ(ced_output.ced_object[FBK_SIDE_RIGHT].length_m, 1.1f);
   EXPECT_FLOAT_EQ(ced_output.ced_object[FBK_SIDE_RIGHT].width_m, 1.2f);
   EXPECT_FLOAT_EQ(ced_output.ced_object[FBK_SIDE_RIGHT].long_pos_m, 1.3f);
   EXPECT_FLOAT_EQ(ced_output.ced_object[FBK_SIDE_RIGHT].lat_pos_m, 1.4f);
   EXPECT_FLOAT_EQ(ced_output.ced_object[FBK_SIDE_RIGHT].speed_mps, 1.5f);
   EXPECT_FLOAT_EQ(ced_output.ced_object[FBK_SIDE_RIGHT].heading_rad, 1.6f);
   EXPECT_EQ(ced_output.ced_object[FBK_SIDE_RIGHT].direction, REAR_DIRECTION);
   EXPECT_FLOAT_EQ(ced_output.ced_object[FBK_SIDE_RIGHT].predicted_lat_pos_m, 1.7f);
   EXPECT_FLOAT_EQ(ced_output.ced_object[FBK_SIDE_RIGHT].ttc_s, 1.8f);
   EXPECT_FLOAT_EQ(ced_output.ced_object[FBK_SIDE_RIGHT].ttp_s, 1.9f);
}

/**
 * Verify direction mapping between core output and generic output. Front direction test
 * \uts{CSCSA-126145} \sdd{CSCSA-121735} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Post_Run_Test, Ced_Map_Object_Direction__set_front_direction)
{
   /** \arrange set core direction to FRONT */
   uint8_t core_direction = FBK_SIDE_FRONT;

   /** \action Run Ced_Map_Object_Direction */
   Ced_Target_Travel_Direction_T generic_direction = Ced_Map_Object_Direction(core_direction);

   /** \assert Verify generic direction is FRONT */
   EXPECT_EQ(generic_direction, FRONT_DIRECTION);
}

/**
 * Verify direction mapping between core output and generic output. Rear direction test
 * \uts{CSCSA-126146} \sdd{CSCSA-121735} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Post_Run_Test, Ced_Map_Object_Direction__set_rear_direction)
{
   /** \arrange set core direction to REAR */
   uint8_t core_direction = FBK_SIDE_REAR;

   /** \action Run Ced_Map_Object_Direction */
   Ced_Target_Travel_Direction_T generic_direction = Ced_Map_Object_Direction(core_direction);

   /** \assert Verify generic direction is REAR */
   EXPECT_EQ(generic_direction, REAR_DIRECTION);
}


/**
 * Verify direction mapping between core output and generic output. UNDEFINED direction test
 * \uts{CSCSA-126147} \sdd{CSCSA-121735} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Post_Run_Test, Ced_Map_Object_Direction__set_undefined_direction)
{
   /** \arrange set core direction to UNDEFINED */
   uint8_t core_direction = 42u;

   /** \action Run Ced_Map_Object_Directions */
   Ced_Target_Travel_Direction_T generic_direction = Ced_Map_Object_Direction(core_direction);

   /** \assert Verify generic direction is UNDEFINED */
   EXPECT_EQ(generic_direction, UNDEF_DIRECTION);
}

/**
 * Check that post run reset function resets the crtical object properties accordingly.
 * \uts{CSCSA-126148} \sdd{CSCSA-126136} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Post_Run_Test, Ced_Reset_Critical_Object__check_reset_routine)
{
   Ced_Critical_Object_T ced_object;

   /** \arrange set object parameters to non default */
   ced_object.id                  = 12u;
   ced_object.type                = PA_OBJ_CLASS_CAR;
   ced_object.length_m            = 1.1f;
   ced_object.width_m             = 1.1f;
   ced_object.long_pos_m          = 1.1f;
   ced_object.lat_pos_m           = 1.1f;
   ced_object.speed_mps           = 1.1f;
   ced_object.heading_rad         = 1.1f;
   ced_object.direction           = FRONT_DIRECTION;
   ced_object.predicted_lat_pos_m = 1.1f;
   ced_object.ttc_s               = 1.1f;
   ced_object.ttp_s               = 1.1f;

   /** \action Run function to test */
   Ced_Reset_Critical_Object(&ced_object);

   /** \assert Verify parameters are set to default */
   EXPECT_EQ(ced_object.id, 0u);
   EXPECT_EQ(ced_object.type, PA_OBJ_CLASS_UNKNOWN);
   EXPECT_FLOAT_EQ(ced_object.length_m, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ced_object.width_m, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ced_object.long_pos_m, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ced_object.lat_pos_m, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ced_object.speed_mps, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ced_object.heading_rad, FBK_ZERO_F);
   EXPECT_EQ(ced_object.direction, UNDEF_DIRECTION);
   EXPECT_FLOAT_EQ(ced_object.predicted_lat_pos_m, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ced_object.ttc_s, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(ced_object.ttp_s, FBK_ZERO_F);
   EXPECT_EQ(ced_object.type, PA_OBJ_CLASS_UNKNOWN);
   EXPECT_EQ(ced_object.direction, UNDEF_DIRECTION);
}
