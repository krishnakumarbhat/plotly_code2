/**
 * @file scw_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for SCW unit tests
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44522}
 */

#include "scw_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_math.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "scw_post_run.c"
}

/*
 * Tests filling the SCW output for active objects on both sides.
 * \uts{CSCSA-44523} \sdd{SF-8156} \testtype{positive}
 */
TEST_F(Scw_Post_Run_Test, Scw_Post_Run__valid_objects_both_sides)
{

   /** \arrange Set up object types. Also SCW state set to ACTIVE */
   *p_scw_current_state                        = SCW_STATE_ACTIVE;
   p_scw_core_output->obj_type[FBK_SIDE_LEFT]  = SCW_OBJECT_TYPE_DYNAMIC;
   p_scw_core_output->obj_type[FBK_SIDE_RIGHT] = SCW_OBJECT_TYPE_DYNAMIC;

   /** \action Call post run function. */
   Scw_Post_Run(&scw_instance, &scw_input, &scw_output);

   /** \assert Verify that output is filled correctly. */
   EXPECT_EQ(scw_output.scw_object_type_left, SCW_OBJECT_TYPE_DYNAMIC);
   EXPECT_EQ(scw_output.scw_object_type_right, SCW_OBJECT_TYPE_DYNAMIC);
}

/*
 * Tests filling the SCW output for active guardrails on both sides.
 * \uts{CSCSA-44524} \sdd{SF-8156} \testtype{positive}
 */
TEST_F(Scw_Post_Run_Test, Scw_Post_Run__valid_guardrails_both_sides)
{

   /** \arrange Set up object types. Also SCW state set to ACTIVE */
   *p_scw_current_state                        = SCW_STATE_ACTIVE;
   p_scw_core_output->obj_type[FBK_SIDE_LEFT]  = SCW_OBJECT_TYPE_GUARDRAIL;
   p_scw_core_output->obj_type[FBK_SIDE_RIGHT] = SCW_OBJECT_TYPE_GUARDRAIL;

   /** \action Call post run function. */
   Scw_Post_Run(&scw_instance, &scw_input, &scw_output);

   /** \assert Verify that output is filled correctly. */
   EXPECT_EQ(scw_output.scw_object_type_left, SCW_OBJECT_TYPE_GUARDRAIL);
   EXPECT_EQ(scw_output.scw_object_type_right, SCW_OBJECT_TYPE_GUARDRAIL);
}

/*
 * Tests filling the SCW output fo lateral distance changed where the guardrails moving closure ego.
 * \uts{CSCSA-101408} \sdd{SF-8156} \testtype{positive}
 */
TEST_F(Scw_Post_Run_Test, Scw_Post_Run__valid_guardrails_both_sides_guardrails_moving_closure_ego)
{
   /** \arrange Set up object types.Also, SCW State set to ACTIVE */
   *p_scw_current_state                                    = SCW_STATE_ACTIVE;
   p_scw_core_output->obj_type[FBK_SIDE_LEFT]              = SCW_OBJECT_TYPE_GUARDRAIL;
   p_scw_core_output->obj_type[FBK_SIDE_RIGHT]             = SCW_OBJECT_TYPE_GUARDRAIL;
   p_scw_core_output->obj_lateral_velocity[FBK_SIDE_LEFT]  = 2.0f;
   p_scw_core_output->obj_lateral_velocity[FBK_SIDE_RIGHT] = -4.0f;

   /** \action Call post run functiontance changed. */
   Scw_Post_Run(&scw_instance, &scw_input, &scw_output);

   /** \assert Verify that output is filled correctly each time. */
   EXPECT_EQ(scw_output.scw_object_vy_left, -p_scw_core_output->obj_lateral_velocity[FBK_SIDE_LEFT]);
   EXPECT_EQ(scw_output.scw_object_vy_right, -p_scw_core_output->obj_lateral_velocity[FBK_SIDE_RIGHT]);
}

/**
 * Tests transformation from vcs to bmw coordinate system for properties of object on left side.
 * \uts{CSCSA-44526} \sdd{SF-8166} \testtype{positive}
 */
TEST_F(Scw_Post_Run_Test, Scw_Transformation_To_BMW_Coord_Sys_Left__transform_properties_on_left_side_to_bmw_coordinate_system)
{
   /** \arrange Set up object pa_data. */
   float32_T val                      = 5.0f;
   p_vehicle_data->rear_axle_position = 1.0f;
   scw_output.scw_object_px_left      = val;
   scw_output.scw_object_py_left      = val;
   scw_output.scw_object_vy_left      = val;
   scw_output.scw_object_ay_left      = val;
   scw_output.scw_object_heading_left = val;

   /** \action transformation for left side. */
   Scw_Transformation_To_BMW_Coord_Sys_Left(&scw_output, p_vehicle_data);

   /** \assert Verify that output is filled correctly. */
   EXPECT_FLOAT_EQ(scw_output.scw_object_px_left, val - p_vehicle_data->rear_axle_position);
   EXPECT_FLOAT_EQ(scw_output.scw_object_py_left, -val);
   EXPECT_FLOAT_EQ(scw_output.scw_object_vy_left, -val);
   EXPECT_FLOAT_EQ(scw_output.scw_object_ay_left, -val);
   EXPECT_FLOAT_EQ(scw_output.scw_object_heading_left, -val);
}

/*
 * Tests transformation from vcs to bmw coordinate system for properties of object on right side.
 * \uts{CSCSA-44527} \sdd{SF-8171} \testtype{positive}
 */
TEST_F(Scw_Post_Run_Test, Scw_Transformation_To_BMW_Coord_Sys_Right__transform_properties_on_right_side_to_bmw_coordinate_system)
{
   /** \arrange Set up object pa_data. */
   float32_T val                       = 5.0f;
   p_vehicle_data->rear_axle_position  = 1.0f;
   scw_output.scw_object_px_right      = val;
   scw_output.scw_object_py_right      = val;
   scw_output.scw_object_vy_right      = val;
   scw_output.scw_object_ay_right      = val;
   scw_output.scw_object_heading_right = val;

   /** \action transformation for right side. */
   Scw_Transformation_To_BMW_Coord_Sys_Right(&scw_output, p_vehicle_data);

   /** \assert Verify that output is filled correctly. */
   EXPECT_FLOAT_EQ(scw_output.scw_object_px_right, val - p_vehicle_data->rear_axle_position);
   EXPECT_FLOAT_EQ(scw_output.scw_object_py_right, -val);
   EXPECT_FLOAT_EQ(scw_output.scw_object_vy_right, -val);
   EXPECT_FLOAT_EQ(scw_output.scw_object_ay_right, -val);
   EXPECT_FLOAT_EQ(scw_output.scw_object_heading_right, -val);
}

/*
 * Tests transformation for guardrail information on left side. Expect that mapping is done correctly.
 * \uts{CSCSA-44528} \sdd{SF-8170} \testtype{positive}
 */
TEST_F(Scw_Post_Run_Test, Scw_Guardrail_Transformation_To_BMW_Coord_Sys_Left__test_default_vals_left_side)
{
   /** \arrange Set up guardrail object pa_data. */
   float32_T val                      = 5.0f;
   scw_output.scw_object_vy_left      = val;
   scw_output.scw_object_ay_left      = val;
   scw_output.scw_object_heading_left = val;


   /** \action transformation for left side. */
   Scw_Guardrail_Transformation_To_BMW_Coord_Sys_Left(&scw_output);

   /** \assert Verify that output is filled correctly. */
   EXPECT_FLOAT_EQ(scw_output.scw_object_vy_left, -val);
   EXPECT_FLOAT_EQ(scw_output.scw_object_ay_left, -val);
   EXPECT_FLOAT_EQ(scw_output.scw_object_heading_left, -val);
}

/*
 * Tests transformation for guardrail information on right side. Expect that mapping is done correctly.
 * \uts{CSCSA-44529} \sdd{SF-8169} \testtype{positive}
 */
TEST_F(Scw_Post_Run_Test, Scw_Guardrail_Transformation_To_BMW_Coord_Sys_Right__test_default_vals_right_side)
{
   /** \arrange Set up guardrail object pa_data. */
   float32_T val                       = 5.0f;
   scw_output.scw_object_vy_right      = val;
   scw_output.scw_object_ay_right      = val;
   scw_output.scw_object_heading_right = val;


   /** \action transformation for right side. */
   Scw_Guardrail_Transformation_To_BMW_Coord_Sys_Right(&scw_output);

   /** \assert Verify that output is filled correctly. */
   EXPECT_FLOAT_EQ(scw_output.scw_object_vy_right, -val);
   EXPECT_FLOAT_EQ(scw_output.scw_object_ay_right, -val);
   EXPECT_FLOAT_EQ(scw_output.scw_object_heading_right, -val);
}


/*
 * Tests mapping of dynamic object properties to the feature output. Here an dynamic object is existing on the left side, thus the
 * object properties on the left side of the ego are expected to be updated. \uts{CSCSA-44532} \sdd{SF-8183} \testtype{positive}
 */
TEST_F(Scw_Post_Run_Test, Scw_Set_Dynamic_Object_Output__dynamic_obj_on_left_side)
{
   /** \arrange Set up scw output to default except ttc and object properties to non default. */

   scw_output.scw_object_ttc_left                                               = 1.0f;
   p_scw_core_output->obj_index[FBK_SIDE_LEFT]                                  = 1u;
   p_scw_core_output->obj_type[FBK_SIDE_LEFT]                                   = SCW_OBJECT_TYPE_DYNAMIC;
   p_scw_core_output->obj_id[FBK_SIDE_LEFT]                                     = 42u;
   p_scw_core_output->obj_unique_id[FBK_SIDE_LEFT]                              = 42u;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_LEFT]].vcs_pos     = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_LEFT]].width       = 2.2f;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_LEFT]].length      = 4.0f;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_LEFT]].vcs_heading = PI;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_LEFT]].vcs_vel     = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_LEFT]].vcs_accel   = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_LEFT]].existence_probability = 0.95f;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_LEFT]].age                   = 15u;

   /** \action Call dynamic object routine. */
   Scw_Set_Dynamic_Object_Output(&scw_output, &scw_instance, FBK_SIDE_LEFT);

   /** \assert Verify that mapping is done correctly. */
   EXPECT_TRUE(scw_output.scw_object_type_left != FBK_ZERO_UINT);
   EXPECT_TRUE(scw_output.scw_object_id_left != FBK_ZERO_UINT);
   EXPECT_TRUE(scw_output.scw_object_unique_id_left != FBK_ZERO_UINT);
   EXPECT_TRUE(scw_output.scw_object_px_left != FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_py_left != FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_width_left != FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_length_left != FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_heading_left != FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_ttc_left == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_vx_left != FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_vy_left != FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_ax_left != FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_ay_left != FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_existance_probability_left != FBK_ZERO_UINT);
   EXPECT_TRUE(scw_output.scw_object_age_left != FBK_ZERO_UINT);
}


/*
 * Tests mapping of dynamic object properties to the feature output. Here an dynamic object is existing on the right side, thus the
 * object properties on the right side of the ego are expected to be updated. \uts{CSCSA-44533} \sdd{SF-8183} \testtype{positive}
 */
TEST_F(Scw_Post_Run_Test, Scw_Set_Dynamic_Object_Output__dynamic_obj_on_right_side)
{
   /** \arrange Set up scw output to default except ttc and object properties to non default. */
   scw_output.scw_object_ttc_left                                                = 1.0f;
   p_scw_core_output->obj_index[FBK_SIDE_RIGHT]                                  = 1u;
   p_scw_core_output->obj_type[FBK_SIDE_RIGHT]                                   = SCW_OBJECT_TYPE_DYNAMIC;
   p_scw_core_output->obj_id[FBK_SIDE_RIGHT]                                     = 42u;
   p_scw_core_output->obj_unique_id[FBK_SIDE_RIGHT]                              = 42u;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].vcs_pos     = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].width       = 2.2f;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].length      = 4.0f;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].vcs_heading = PI;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].vcs_vel     = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].vcs_accel   = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].existence_probability = 0.95f;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].age                   = 15u;

   /** \action Call dynamic object routine. */
   Scw_Set_Dynamic_Object_Output(&scw_output, &scw_instance, FBK_SIDE_RIGHT);

   /** \assert Verify that mapping is done correctly. */
   EXPECT_TRUE(scw_output.scw_object_type_right != FBK_ZERO_UINT);
   EXPECT_TRUE(scw_output.scw_object_id_right != FBK_ZERO_UINT);
   EXPECT_TRUE(scw_output.scw_object_unique_id_right != FBK_ZERO_UINT);
   EXPECT_TRUE(scw_output.scw_object_px_right != FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_py_right != FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_width_right != FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_length_right != FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_heading_right != FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_ttc_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_vx_right != FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_vy_right != FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_ax_right != FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_ay_right != FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_existance_probability_right != FBK_ZERO_UINT);
   EXPECT_TRUE(scw_output.scw_object_age_right != FBK_ZERO_UINT);
}


/*
 * Tests mapping of dynamic object properties to the feature output. Here no valid side is given, but due to a magic bit flip we
 * were able to step into this function with invalid pa_data. Expect that no corruption is happening. \uts{CSCSA-44534}
 * \sdd{SF-8183} \testtype{negative}
 */
TEST_F(Scw_Post_Run_Test, Scw_Set_Dynamic_Object_Output__no_valid_side_given)
{
   /** \arrange Set up scw output to default except ttc and object properties to non default. */
   scw_output.scw_object_ttc_left                                                = 1.0f;
   p_scw_core_output->obj_index[FBK_SIDE_RIGHT]                                  = 1u;
   p_scw_core_output->obj_type[FBK_SIDE_RIGHT]                                   = SCW_OBJECT_TYPE_DYNAMIC;
   p_scw_core_output->obj_id[FBK_SIDE_RIGHT]                                     = 42u;
   p_scw_core_output->obj_unique_id[FBK_SIDE_RIGHT]                              = 42u;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].vcs_pos     = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].width       = 2.2f;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].length      = 4.0f;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].vcs_heading = PI;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].vcs_vel     = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].vcs_accel   = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].existence_probability = 0.95f;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].age                   = 15u;
   /** \action Call dynamic object routine. */
   Scw_Set_Dynamic_Object_Output(&scw_output, &scw_instance, FBK_NUMBER_OF_SIDES);

   /** \assert Verify that everything stays the same. */
   EXPECT_TRUE(scw_output.scw_object_type_right == FBK_ZERO_UINT);
   EXPECT_TRUE(scw_output.scw_object_id_right == FBK_ZERO_UINT);
   EXPECT_TRUE(scw_output.scw_object_unique_id_right == FBK_ZERO_UINT);
   EXPECT_TRUE(scw_output.scw_object_px_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_py_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_width_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_length_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_heading_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_ttc_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_vx_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_vy_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_ax_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_ay_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_existance_probability_right == FBK_ZERO_UINT);
   EXPECT_TRUE(scw_output.scw_object_age_right == FBK_ZERO_UINT);
}


/*
 * Tests mapping of guardrail object properties to the feature output. Here a dangerous guardrail object is existing on the left
 * side, thus the object properties on the left side of the ego are expected to be updated. \uts{CSCSA-44535} \sdd{SF-8182}
 * \testtype{positive}
 */
TEST_F(Scw_Post_Run_Test, Scw_Set_Guardrail_Object_Output__dangerous_guardrail_on_the_left)
{
   /** \arrange Set up scw output to default except ttc and object properties to non default. */
   p_scw_core_output->obj_index[FBK_SIDE_LEFT]                 = 1u;
   p_scw_core_output->obj_id[FBK_SIDE_LEFT]                    = 3u;
   p_scw_core_output->obj_unique_id[FBK_SIDE_LEFT]             = 3u;
   p_scw_core_output->obj_type[FBK_SIDE_LEFT]                  = SCW_OBJECT_TYPE_GUARDRAIL;
   pa_data.guardrail_data[FBK_SIDE_LEFT].lat_pos               = -1.0f;
   pa_data.guardrail_data[FBK_SIDE_LEFT].existence_probability = 0.95f;
   pa_data.guardrail_data[FBK_SIDE_LEFT].age                   = 15u;

   /** \action Call guardrail object routine. */
   Scw_Set_Guardrail_Object_Output(&scw_output, &scw_instance, FBK_SIDE_LEFT);

   /** \assert Verify that mapping is done correctly. */
   EXPECT_EQ(scw_output.scw_object_type_left, SCW_OBJECT_TYPE_GUARDRAIL);
   EXPECT_EQ(scw_output.scw_object_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(scw_output.scw_object_unique_id_left, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(scw_output.scw_object_py_left, -pa_data.guardrail_data[FBK_SIDE_LEFT].lat_pos);
   EXPECT_EQ(scw_output.scw_object_existance_probability_left, 100 * pa_data.guardrail_data[FBK_SIDE_LEFT].existence_probability);
   EXPECT_EQ(scw_output.scw_object_age_left, pa_data.guardrail_data[FBK_SIDE_LEFT].age);
}


/*
 * Tests mapping of guardrail object properties to the feature output. Here a dangerous guardrail object is existing on the right
 * side, thus the object properties on the right side of the ego are expected to be updated. \uts{CSCSA-44536} \sdd{SF-8182}
 * \testtype{positive}
 */
TEST_F(Scw_Post_Run_Test, Scw_Set_Guardrail_Object_Output__dangerous_guardrail_on_the_right)
{
   /** \arrange Set up scw output to default except ttc and object properties to non default. */
   p_scw_core_output->obj_index[FBK_SIDE_RIGHT]                 = 1u;
   p_scw_core_output->obj_id[FBK_SIDE_RIGHT]                    = 42u;
   p_scw_core_output->obj_unique_id[FBK_SIDE_RIGHT]             = 42u;
   p_scw_core_output->obj_type[FBK_SIDE_RIGHT]                  = SCW_OBJECT_TYPE_GUARDRAIL;
   pa_data.guardrail_data[FBK_SIDE_RIGHT].lat_pos               = 1.0f;
   pa_data.guardrail_data[FBK_SIDE_RIGHT].existence_probability = 0.95f;
   pa_data.guardrail_data[FBK_SIDE_RIGHT].age                   = 15u;

   /** \action Call guardrail object routine. */
   Scw_Set_Guardrail_Object_Output(&scw_output, &scw_instance, FBK_SIDE_RIGHT);

   /** \assert Verify that mapping is done correctly. */
   EXPECT_EQ(scw_output.scw_object_type_right, SCW_OBJECT_TYPE_GUARDRAIL);
   EXPECT_EQ(scw_output.scw_object_id_right, FBK_ZERO_UINT);
   EXPECT_EQ(scw_output.scw_object_unique_id_right, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(scw_output.scw_object_py_right, -pa_data.guardrail_data[FBK_SIDE_RIGHT].lat_pos);
   EXPECT_EQ(scw_output.scw_object_existance_probability_right, 100 * pa_data.guardrail_data[FBK_SIDE_RIGHT].existence_probability);
   EXPECT_EQ(scw_output.scw_object_age_right, pa_data.guardrail_data[FBK_SIDE_RIGHT].age);
}


/*
 * Tests mapping of guardrail object properties to the feature output. Here a dangerous guardrail object is existing but we are
 * giving the function a non existing side. Thus we are hoping, that the scw output is not corrupted \uts{CSCSA-44537}
 * \sdd{SF-8182} \testtype{negative}
 */
TEST_F(Scw_Post_Run_Test, Scw_Set_Guardrail_Object_Output__invalid_data_is_not_corrupting_our_beautiful_default_output)
{
   /** \arrange Set up scw output to default except ttc and object properties to non default. */
   scw_output.scw_object_ttc_left                                                = 1.0f;
   p_scw_core_output->obj_index[FBK_SIDE_RIGHT]                                  = 1u;
   p_scw_core_output->obj_type[FBK_SIDE_RIGHT]                                   = SCW_OBJECT_TYPE_GUARDRAIL;
   p_scw_core_output->obj_id[FBK_SIDE_RIGHT]                                     = 42u;
   p_scw_core_output->obj_unique_id[FBK_SIDE_RIGHT]                              = 42u;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].vcs_pos     = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].width       = 2.2f;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].length      = 4.0f;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].vcs_heading = PI;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].vcs_heading = PI;
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].vcs_vel     = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   pa_data.object_data[p_scw_core_output->obj_index[FBK_SIDE_RIGHT]].vcs_accel   = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   pa_data.guardrail_data[FBK_SIDE_RIGHT].existence_probability                  = 0.95f;
   pa_data.guardrail_data[FBK_SIDE_RIGHT].age                                    = 15u;

   /** \action Call guardrail object routine. */
   Scw_Set_Guardrail_Object_Output(&scw_output, &scw_instance, FBK_NUMBER_OF_SIDES);
   /** \assert Verify that mapping is done correctly. */
   EXPECT_TRUE(scw_output.scw_object_type_right == FBK_ZERO_UINT);
   EXPECT_TRUE(scw_output.scw_object_id_right == FBK_ZERO_UINT);
   EXPECT_TRUE(scw_output.scw_object_unique_id_right == FBK_ZERO_UINT);
   EXPECT_TRUE(scw_output.scw_object_px_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_py_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_width_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_length_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_heading_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_ttc_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_vx_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_vy_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_ax_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_ay_right == FBK_ZERO_F);
   EXPECT_TRUE(scw_output.scw_object_existance_probability_right == FBK_ZERO_UINT);
   EXPECT_TRUE(scw_output.scw_object_age_right == FBK_ZERO_UINT);
}


/*
 * Tests the reset functionality of SCW output. Check that the mapping is done correctly.
 * \uts{CSCSA-44538} \sdd{SF-8184} \testtype{negative}
 */
TEST_F(Scw_Post_Run_Test, Scw_Reset_Output__check_default_reset)
{
   /** \arrange Set up scw output non default values. */
   scw_output.f_scw_enabled           = FBK_TRUE;
   scw_output.f_scw_dyn_enabled       = FBK_TRUE;
   scw_output.f_scw_guardrail_enabled = FBK_TRUE;

   /* Reset object properties on the left side of ego. */
   scw_output.scw_object_type_left                  = 42u;
   scw_output.scw_object_id_left                    = 42u;
   scw_output.scw_object_px_left                    = 42.0f;
   scw_output.scw_object_py_left                    = 42.0f;
   scw_output.scw_object_width_left                 = 42.0f;
   scw_output.scw_object_length_left                = 42.0f;
   scw_output.scw_object_heading_left               = 42.0f;
   scw_output.scw_object_ttc_left                   = 42.0f;
   scw_output.scw_object_vx_left                    = 42.0f;
   scw_output.scw_object_vy_left                    = 42.0f;
   scw_output.scw_object_ax_left                    = 42.0f;
   scw_output.scw_object_ay_left                    = 42.0f;
   scw_output.scw_object_existance_probability_left = 42u;
   scw_output.scw_object_age_left                   = 42u;
   /*Added for BMW Sp25*/
   scw_output.scw_object_criticality_left_output   = FBK_ZERO_UINT;
   scw_output.scw_object_time_stamp_left_output    = FBK_ZERO_F;
   scw_output.scw_object_fallback_time_left_output = FBK_ZERO_F;
   scw_output.scw_object_ttp_left                  = FBK_ZERO_F;
   /* Reset object properties on the right side of ego. */
   scw_output.scw_object_type_right                  = 42u;
   scw_output.scw_object_id_right                    = 42u;
   scw_output.scw_object_px_right                    = 42.0f;
   scw_output.scw_object_py_right                    = 42.0f;
   scw_output.scw_object_width_right                 = 42.0f;
   scw_output.scw_object_length_right                = 42.0f;
   scw_output.scw_object_heading_right               = 42.0f;
   scw_output.scw_object_ttc_right                   = 42.0f;
   scw_output.scw_object_vx_right                    = 42.0f;
   scw_output.scw_object_vy_right                    = 42.0f;
   scw_output.scw_object_ax_right                    = 42.0f;
   scw_output.scw_object_ay_right                    = 42.0f;
   scw_output.scw_object_existance_probability_right = 42u;
   scw_output.scw_object_age_right                   = 42u;
   /*Added for BMW Sp25*/
   scw_output.scw_object_criticality_right_output   = FBK_ZERO_UINT;
   scw_output.scw_object_time_stamp_right_output    = FBK_ZERO_F;
   scw_output.scw_object_fallback_time_right_output = FBK_ZERO_F;
   scw_output.scw_object_ttp_right                  = FBK_ZERO_F;

   /** \action Call reset routine. */
   Scw_Reset_Output(&scw_output, p_scw_calibration);

   /** \assert Verify that mapping is done correctly. */
   EXPECT_FALSE(scw_output.f_scw_enabled);
   EXPECT_FALSE(scw_output.f_scw_dyn_enabled);
   EXPECT_FALSE(scw_output.f_scw_guardrail_enabled);
   EXPECT_FLOAT_EQ(scw_output.scw_object_px_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(scw_output.scw_object_py_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(scw_output.scw_object_width_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(scw_output.scw_object_length_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(scw_output.scw_object_heading_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(scw_output.scw_object_ttc_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(scw_output.scw_object_vx_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(scw_output.scw_object_vy_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(scw_output.scw_object_ax_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(scw_output.scw_object_ay_left, FBK_ZERO_F);
   EXPECT_EQ(scw_output.scw_object_type_left, FBK_ZERO_UINT);
   EXPECT_EQ(scw_output.scw_object_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(scw_output.scw_object_existance_probability_left, FBK_ZERO_UINT);
   EXPECT_EQ(scw_output.scw_object_age_left, FBK_ZERO_UINT);

   /* Added for BMW Sp25 */
   EXPECT_EQ(scw_output.scw_object_criticality_left_output, FBK_ZERO_UINT);
   EXPECT_EQ(scw_output.scw_object_time_stamp_left_output, FBK_ZERO_UINT);
   EXPECT_EQ(scw_output.scw_object_fallback_time_left_output, FBK_ZERO_UINT);
   EXPECT_EQ(scw_output.scw_object_ttp_left, p_scw_calibration->k_scw_ttp_default);
   /* Right Object Information */
   EXPECT_FLOAT_EQ(scw_output.scw_object_px_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(scw_output.scw_object_py_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(scw_output.scw_object_width_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(scw_output.scw_object_length_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(scw_output.scw_object_heading_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(scw_output.scw_object_ttc_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(scw_output.scw_object_vx_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(scw_output.scw_object_vy_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(scw_output.scw_object_ax_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(scw_output.scw_object_ay_right, FBK_ZERO_F);
   EXPECT_EQ(scw_output.scw_object_type_right, FBK_ZERO_UINT);
   EXPECT_EQ(scw_output.scw_object_id_right, FBK_ZERO_UINT);
   EXPECT_EQ(scw_output.scw_object_existance_probability_right, FBK_ZERO_UINT);
   EXPECT_EQ(scw_output.scw_object_age_right, FBK_ZERO_UINT);
   /* Added for BMW Sp25 */
   EXPECT_EQ(scw_output.scw_object_criticality_right_output, FBK_ZERO_UINT);
   EXPECT_EQ(scw_output.scw_object_time_stamp_right_output, FBK_ZERO_UINT);
   EXPECT_EQ(scw_output.scw_object_fallback_time_right_output, FBK_ZERO_UINT);
   EXPECT_EQ(scw_output.scw_object_ttp_right, p_scw_calibration->k_scw_ttp_default);
}

/* Tests the reset functionality of SCW output. Check that the mapping is done correctly via initialization
 * routine. \uts{CSCSA-44539} \sdd{SF-8187} \testtype{positive}
 */
TEST_F(Scw_Post_Run_Test, Scw_Post_Run_Init__check_default_reset)
{
   /** \arrange Set up scw output non default values. */
   scw_output.f_scw_enabled           = FBK_TRUE;
   scw_output.f_scw_dyn_enabled       = FBK_TRUE;
   scw_output.f_scw_guardrail_enabled = FBK_TRUE;

   Scw_Last_Lateral_Distance[FBK_SIDE_LEFT] = 20.0f;
   scw_output.scw_object_type_left          = 42u;
   scw_output.scw_object_type_right         = 42u;

   /** \action Call reset routine. */
   Scw_Reset_Output(&scw_output, p_scw_calibration);

   /** \assert Verify that mapping is done correctly. */
   EXPECT_EQ(scw_output.scw_object_type_left, FBK_ZERO_UINT);
   EXPECT_EQ(scw_output.scw_object_type_right, FBK_ZERO_UINT);
}

/*
 * Tests the reset functionality of persistent scw pa_data. Check that the mapping is done correctly.
 * \uts{CSCSA-44540} \sdd{SF-8188} \testtype{positive}
 */
TEST_F(Scw_Post_Run_Test, Scw_Reset_Post_Run_Persistent__check_default_reset)
{
   /** \arrange Set up scw output non default values. */
   Scw_Last_Lateral_Distance[FBK_SIDE_LEFT]  = 20.0f;
   Scw_Last_Lateral_Distance[FBK_SIDE_RIGHT] = 30.0f;

   /** \action Call reset routine. */
   Scw_Reset_Post_Run_Persistent(Scw_Last_Lateral_Distance);

   /** \assert Verify that mapping is done correctly. */
   EXPECT_FLOAT_EQ(Scw_Last_Lateral_Distance[FBK_SIDE_LEFT], FBK_ZERO_F);
   EXPECT_FLOAT_EQ(Scw_Last_Lateral_Distance[FBK_SIDE_RIGHT], FBK_ZERO_F);
}
