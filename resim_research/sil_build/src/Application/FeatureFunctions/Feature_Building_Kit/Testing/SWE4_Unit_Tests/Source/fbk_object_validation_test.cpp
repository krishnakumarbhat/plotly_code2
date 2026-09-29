/**
 * @file fbk_object_validation_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for FBK object validation.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42237}
 */

#include "fbk_object_validation_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.c"
#include "ml_vector_2d_t.h"
#include "pa_context.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}


/**
 * Checks whether an object is seen in sensor field of view. Here the object is seen by rear right sensor. Thus true is expected.
 * \uts{CSCSA-42423} \sdd{SF-4132} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Is_Obj_In_Any_Sensor_Fov__object_in_rr_fov)
{
   /** \arrange set object with index 1 to rear right sensor. */
   boolean_T res;
   uint8_t idx                            = 1;
   object_data[idx].f_is_in_rr_sensor_fov = FBK_TRUE;

   /** \action executes function to test. */
   res = Fbk_Is_Obj_In_Any_Sensor_Fov(&object_data[idx]);

   /** \assert expect that true is returned. */
   EXPECT_TRUE(res);
}


/**
 * Checks whether an object is seen in sensor field of view. Here the object is seen by rear left sensor. Thus true is expected.
 * \uts{CSCSA-42424} \sdd{SF-4132} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Is_Obj_In_Any_Sensor_Fov__object_in_rl_fov)
{
   /** \arrange set object with index 1 to rear left sensor. */
   boolean_T res;
   uint8_t idx                            = 1;
   object_data[idx].f_is_in_rl_sensor_fov = FBK_TRUE;

   /** \action executes function to test. */
   res = Fbk_Is_Obj_In_Any_Sensor_Fov(&object_data[idx]);

   /** \assert expect that true is returned. */
   EXPECT_TRUE(res);
}


/**
 * Checks whether an object is seen in sensor field of view. Here the object is seen by front left sensor. Thus true is expected.
 * \uts{CSCSA-42425} \sdd{SF-4132} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Is_Obj_In_Any_Sensor_Fov__object_in_fl_fov)
{
   /** \arrange set object with index 1 to front left sensor. */
   boolean_T res;
   uint8_t idx                            = 1;
   object_data[idx].f_is_in_fl_sensor_fov = FBK_TRUE;

   /** \action executes function to test. */
   res = Fbk_Is_Obj_In_Any_Sensor_Fov(&object_data[idx]);

   /** \assert expect that true is returned. */
   EXPECT_TRUE(res);
}

/**
 * Checks whether an object is seen in sensor field of view. Here the object is seen by front right sensor. Thus true is expected.
 * \uts{CSCSA-42426} \sdd{SF-4132} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Is_Obj_In_Any_Sensor_Fov__object_in_fr_fov)
{
   /** \arrange set object with index 1 to front right sensor. */
   boolean_T res;
   uint8_t idx                            = 1;
   object_data[idx].f_is_in_fr_sensor_fov = FBK_TRUE;

   /** \action executes function to test. */
   res = Fbk_Is_Obj_In_Any_Sensor_Fov(&object_data[idx]);

   /** \assert expect that true is returned. */
   EXPECT_TRUE(res);
}


/**
 * Checks whether an object is seen in sensor field of view. Here the object is not seen by any sensor. Thus false is expected.
 * \uts{CSCSA-42427} \sdd{SF-4132} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Is_Obj_In_Any_Sensor_Fov__object_not_in_any_fov)
{
   /** \arrange set object with index 1 to no sensor fov. */
   boolean_T res;
   uint8_t idx = 1;

   /** \action executes function to test. */
   res = Fbk_Is_Obj_In_Any_Sensor_Fov(&object_data[idx]);

   /** \assert expect that false is returned. */
   EXPECT_FALSE(res);
}

#ifndef NDEBUG

/**
 * Check if Fbk_Is_Obj_In_Any_Sensor_Fov is throwing an exception when context is NULL.
 * \uts{CSCSA-42428} \sdd{SF-4132} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Is_Obj_In_Any_Sensor_Fov__context_is_null)
{
   EXPECT_DEATH(
      {
         /** \arrange set up NULL pointer */
         /** \action call function to test */
         Fbk_Is_Obj_In_Any_Sensor_Fov(NULL);
         /** \assert check that assertion is thrown */
      },
      ".*p_object_data.*");
}
#endif

/**
 * Checks whether an object status is plausible. Here the object is not in any sensor field of view and is additionally coasting.
 * Thus true is expected. \uts{CSCSA-42429} \sdd{SF-4131} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Is_Obj_Coasted_Status_Implausible__object_status_is_implausible)
{
   /** \arrange set object to coasted status. */
   boolean_T res;
   uint8_t idx             = 1;
   object_data[idx].status = PA_OBJ_STATUS_COASTED;

   /** \action executes function to test. */
   res = Fbk_Is_Obj_Coasted_Status_Implausible(&object_data[idx]);

   /** \assert expect that true is returned. */
   EXPECT_TRUE(res);
}


/**
 * Checks whether an object status is plausible. Here the object is not in any sensor field of view but is mature. Thus false is
 * expected. \uts{CSCSA-42430} \sdd{SF-4131} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Is_Obj_Coasted_Status_Implausible__object_status_mature_and_not_in_fov)
{
   /** \arrange set object to mature status. */
   boolean_T res;
   uint8_t idx             = 1;
   object_data[idx].status = PA_OBJ_STATUS_MATURE;

   /** \action executes function to test. */
   res = Fbk_Is_Obj_Coasted_Status_Implausible(&object_data[idx]);

   /** \assert expect that false is returned. */
   EXPECT_FALSE(res);
}


/**
 * Checks whether an object status is plausible. Here the object is in sensor field of view of rear right sensor and is coasting.
 * Thus coasting is expected to be real and status is plausible. \uts{CSCSA-42431} \sdd{SF-4131} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Is_Obj_Coasted_Status_Implausible__object_status_expected_as_really_coasted)
{
   /** \arrange set object status to really coasted. */
   boolean_T res;
   uint8_t idx                            = 1;
   object_data[idx].status                = PA_OBJ_STATUS_COASTED;
   object_data[idx].f_is_in_rr_sensor_fov = FBK_TRUE;

   /** \action executes function to test. */
   res = Fbk_Is_Obj_Coasted_Status_Implausible(&object_data[idx]);

   /** \assert expect that false is returned. */
   EXPECT_FALSE(res);
}


#ifndef NDEBUG

/**
 * Check if Fbk_Is_Obj_Coasted_Status_Implausible is throwing an exception when context is NULL.
 * \uts{CSCSA-42432} \sdd{SF-4131} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Is_Obj_Coasted_Status_Implausible__context_is_null)
{
   EXPECT_DEATH(
      {
         /** \arrange set up NULL pointer */
         /** \action call function to test */
         Fbk_Is_Obj_Coasted_Status_Implausible(NULL);

         /** \assert check that assertion is thrown */
      },
      ".*p_object_data.*");
}
#endif


/**
 * Set up an object whose data is non default and check whether defaults are set correctly.
 * \uts{CSCSA-42433} \sdd{SF-4175} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Reset_Object_Data__works_properly)
{
   /** \arrange set object data to non default */
   fbk_object_data.index                         = 12u;
   fbk_object_data.id                            = 34u;
   fbk_object_data.status                        = PA_OBJ_STATUS_MATURE;
   fbk_object_data.age                           = FBK_ONE_UINT;
   fbk_object_data.stage_age                     = FBK_ONE_UINT;
   fbk_object_data.fbk_stage_age                 = FBK_ONE_UINT;
   fbk_object_data.existence_probability         = FBK_ONE_F;
   fbk_object_data.vcs_pos.x                     = FBK_ONE_F;
   fbk_object_data.vcs_vel.x                     = FBK_ONE_F;
   fbk_object_data.vcs_vel_rel.x                 = FBK_ONE_F;
   fbk_object_data.vcs_accel.x                   = FBK_ONE_F;
   fbk_object_data.vcs_pos.y                     = FBK_ONE_F;
   fbk_object_data.vcs_vel.y                     = FBK_ONE_F;
   fbk_object_data.vcs_vel_rel.y                 = FBK_ONE_F;
   fbk_object_data.vcs_accel.y                   = FBK_ONE_F;
   fbk_object_data.vcs_heading                   = FBK_ONE_F;
   fbk_object_data.heading_rate                  = FBK_ONE_F;
   fbk_object_data.heading_variance              = FBK_ONE_F;
   fbk_object_data.speed                         = FBK_ONE_F;
   fbk_object_data.eclipse_value                 = FBK_ONE_F;
   fbk_object_data.length                        = FBK_ONE_F;
   fbk_object_data.width                         = FBK_ONE_F;
   fbk_object_data.obj_distance                  = FBK_ONE_F;
   fbk_object_data.obstruction_prob              = FBK_ONE_F;
   fbk_object_data.f_reflection                  = FBK_TRUE;
   fbk_object_data.obj_class                     = PA_OBJ_CLASS_CAR;
   fbk_object_data.class_prob_pedestrian         = FBK_ONE_F;
   fbk_object_data.class_prob_2wheel             = FBK_ONE_F;
   fbk_object_data.class_prob_car                = FBK_ONE_F;
   fbk_object_data.class_prob_truck              = FBK_ONE_F;
   fbk_object_data.id_merged_obj                 = FBK_ONE_INT;
   fbk_object_data.f_merge_occured               = FBK_TRUE;
   fbk_object_data.curvi_coordinates_calc_method = PA_OBJ_CURVI_COORDINATES_SNAIL_TRAIL;
   fbk_object_data.curvi_pos.x                   = FBK_ONE_F;
   fbk_object_data.curvi_vel.x                   = FBK_ONE_F;
   fbk_object_data.curvi_vel_rel.x               = FBK_ONE_F;
   fbk_object_data.curvi_pos.y                   = FBK_ONE_F;
   fbk_object_data.curvi_vel.y                   = FBK_ONE_F;
   fbk_object_data.curvi_vel_rel.y               = FBK_ONE_F;
   fbk_object_data.curvi_heading                 = FBK_ONE_F;
   fbk_object_data.accuracy_heading              = FBK_ONE_F;
   fbk_object_data.f_is_fl_origin_sensor         = FBK_TRUE;
   fbk_object_data.f_is_fr_origin_sensor         = FBK_TRUE;
   fbk_object_data.f_is_rl_origin_sensor         = FBK_TRUE;
   fbk_object_data.f_is_rr_origin_sensor         = FBK_TRUE;
   fbk_object_data.f_is_in_fl_sensor_fov         = FBK_TRUE;
   fbk_object_data.f_is_in_fr_sensor_fov         = FBK_TRUE;
   fbk_object_data.f_is_in_rl_sensor_fov         = FBK_TRUE;
   fbk_object_data.f_is_in_rr_sensor_fov         = FBK_TRUE;
   fbk_object_data.f_stationary                  = FBK_TRUE;
   fbk_object_data.f_moveable                    = FBK_TRUE;
   fbk_object_data.f_stationary_clutter          = FBK_FALSE;

   /** \action executes function to test. */
   Fbk_Reset_Object_Data(&fbk_object_data);

   /** \assert expect that default is set. */
   EXPECT_EQ(fbk_object_data.index, PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(fbk_object_data.id, PA_INVALID_OBJ_ID);
   EXPECT_EQ(fbk_object_data.status, PA_OBJ_STATUS_INVALID);
   EXPECT_EQ(fbk_object_data.age, FBK_ZERO_UINT);
   EXPECT_EQ(fbk_object_data.stage_age, FBK_ZERO_UINT);
   EXPECT_EQ(fbk_object_data.fbk_stage_age, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(fbk_object_data.existence_probability, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.vcs_pos.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.vcs_vel.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.vcs_vel_rel.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.vcs_accel.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.vcs_pos.y, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.vcs_vel.y, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.vcs_vel_rel.y, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.vcs_accel.y, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.vcs_heading, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.heading_rate, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.heading_variance, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.speed, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.eclipse_value, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.length, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.width, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.obj_distance, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.obstruction_prob, FBK_ZERO_F);
   EXPECT_FALSE(fbk_object_data.f_reflection);
   EXPECT_EQ(fbk_object_data.obj_class, PA_OBJ_CLASS_UNKNOWN);
   EXPECT_FLOAT_EQ(fbk_object_data.class_prob_pedestrian, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.class_prob_2wheel, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.class_prob_car, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.class_prob_truck, FBK_ZERO_F);
   EXPECT_EQ(fbk_object_data.id_merged_obj, FBK_ZERO_INT);
   EXPECT_FALSE(fbk_object_data.f_merge_occured);
   EXPECT_EQ(fbk_object_data.curvi_coordinates_calc_method, PA_OBJ_CURVI_COORDINATES_UNKNOWN);
   EXPECT_FLOAT_EQ(fbk_object_data.curvi_pos.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.curvi_pos.y, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.curvi_vel_rel.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.curvi_vel.x, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.curvi_vel.y, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.curvi_vel_rel.y, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.curvi_heading, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(fbk_object_data.accuracy_heading, FBK_ZERO_F);
   EXPECT_FALSE(fbk_object_data.f_is_fl_origin_sensor);
   EXPECT_FALSE(fbk_object_data.f_is_fr_origin_sensor);
   EXPECT_FALSE(fbk_object_data.f_is_rl_origin_sensor);
   EXPECT_FALSE(fbk_object_data.f_is_rr_origin_sensor);
   EXPECT_FALSE(fbk_object_data.f_is_in_fl_sensor_fov);
   EXPECT_FALSE(fbk_object_data.f_is_in_fr_sensor_fov);
   EXPECT_FALSE(fbk_object_data.f_is_in_rl_sensor_fov);
   EXPECT_FALSE(fbk_object_data.f_is_in_rr_sensor_fov);
   EXPECT_FALSE(fbk_object_data.f_stationary);
   EXPECT_FALSE(fbk_object_data.f_moveable);
   EXPECT_TRUE(fbk_object_data.f_stationary_clutter);
}

/**
 * Check that the FBK object data is filled properly from the context data.
 * \uts{CSCSA-42434} \sdd{SF-4176} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Fill_Object_Information__works_properly)
{
   /** \arrange Set up non-default object data in context data. */
   uint8_t obj_index                                    = 12u;
   object_data[obj_index].id                            = 34u;
   object_data[obj_index].status                        = PA_OBJ_STATUS_MATURE;
   object_data[obj_index].age                           = FBK_ONE_UINT;
   object_data[obj_index].stage_age                     = FBK_ONE_UINT;
   object_data[obj_index].fbk_stage_age                 = FBK_ONE_UINT;
   object_data[obj_index].existence_probability         = FBK_ONE_F;
   object_data[obj_index].vcs_pos.x                     = FBK_ONE_F;
   object_data[obj_index].vcs_vel.x                     = FBK_ONE_F;
   object_data[obj_index].vcs_vel_rel.x                 = FBK_ONE_F;
   object_data[obj_index].vcs_accel.x                   = FBK_ONE_F;
   object_data[obj_index].vcs_pos.y                     = FBK_ONE_F;
   object_data[obj_index].vcs_vel.y                     = FBK_ONE_F;
   object_data[obj_index].vcs_vel_rel.y                 = FBK_ONE_F;
   object_data[obj_index].vcs_accel.y                   = FBK_ONE_F;
   object_data[obj_index].vcs_heading                   = FBK_ONE_F;
   object_data[obj_index].heading_rate                  = FBK_ONE_F;
   object_data[obj_index].heading_variance              = FBK_ONE_F;
   object_data[obj_index].speed                         = FBK_ONE_F;
   object_data[obj_index].eclipse_value                 = FBK_ONE_F;
   object_data[obj_index].length                        = FBK_ONE_F;
   object_data[obj_index].width                         = FBK_ONE_F;
   object_data[obj_index].obj_distance                  = FBK_ONE_F;
   object_data[obj_index].obstruction_prob              = FBK_ONE_F;
   object_data[obj_index].f_reflection                  = FBK_TRUE;
   object_data[obj_index].obj_class                     = PA_OBJ_CLASS_CAR;
   object_data[obj_index].class_prob_pedestrian         = FBK_ONE_F;
   object_data[obj_index].class_prob_2wheel             = FBK_ONE_F;
   object_data[obj_index].class_prob_car                = FBK_ONE_F;
   object_data[obj_index].class_prob_truck              = FBK_ONE_F;
   object_data[obj_index].id_merged_obj                 = FBK_ONE_INT;
   object_data[obj_index].f_merge_occured               = FBK_TRUE;
   object_data[obj_index].curvi_coordinates_calc_method = PA_OBJ_CURVI_COORDINATES_SNAIL_TRAIL;
   object_data[obj_index].curvi_pos.x                   = FBK_ONE_F;
   object_data[obj_index].curvi_vel.x                   = FBK_ONE_F;
   object_data[obj_index].curvi_vel_rel.x               = FBK_ONE_F;
   object_data[obj_index].curvi_pos.y                   = FBK_ONE_F;
   object_data[obj_index].curvi_vel.y                   = FBK_ONE_F;
   object_data[obj_index].curvi_vel_rel.y               = FBK_ONE_F;
   object_data[obj_index].curvi_heading                 = FBK_ONE_F;
   object_data[obj_index].accuracy_heading              = FBK_ONE_F;
   object_data[obj_index].f_is_fl_origin_sensor         = FBK_TRUE;
   object_data[obj_index].f_is_fr_origin_sensor         = FBK_TRUE;
   object_data[obj_index].f_is_rl_origin_sensor         = FBK_TRUE;
   object_data[obj_index].f_is_rr_origin_sensor         = FBK_TRUE;
   object_data[obj_index].f_is_in_fl_sensor_fov         = FBK_TRUE;
   object_data[obj_index].f_is_in_fr_sensor_fov         = FBK_TRUE;
   object_data[obj_index].f_is_in_rl_sensor_fov         = FBK_TRUE;
   object_data[obj_index].f_is_in_rr_sensor_fov         = FBK_TRUE;
   object_data[obj_index].f_stationary                  = FBK_TRUE;
   object_data[obj_index].f_moveable                    = FBK_TRUE;
   object_data[obj_index].f_stationary_clutter          = FBK_FALSE;

   /** \action Run function to fill FBK object data. */
   Fbk_Fill_Object_Information(&fbk_object_data, &context, obj_index);

   /** \assert Verify that context data is filled into FBK object data. */
   EXPECT_EQ(fbk_object_data.index, obj_index);
   EXPECT_EQ(fbk_object_data.id, 34u);
   EXPECT_EQ(fbk_object_data.status, PA_OBJ_STATUS_MATURE);
   EXPECT_EQ(fbk_object_data.age, FBK_ONE_UINT);
   EXPECT_EQ(fbk_object_data.stage_age, FBK_ONE_UINT);
   EXPECT_EQ(fbk_object_data.fbk_stage_age, FBK_ZERO_UINT); // not set by Fbk_Fill_Object_Information
   EXPECT_FLOAT_EQ(fbk_object_data.existence_probability, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.vcs_pos.x, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.vcs_vel.x, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.vcs_vel_rel.x, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.vcs_accel.x, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.vcs_pos.y, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.vcs_vel.y, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.vcs_vel_rel.y, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.vcs_accel.y, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.vcs_heading, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.heading_rate, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.heading_variance, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.speed, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.eclipse_value, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.length, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.width, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.obj_distance, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.obstruction_prob, FBK_ONE_F);
   EXPECT_TRUE(fbk_object_data.f_reflection);
   EXPECT_EQ(fbk_object_data.obj_class, PA_OBJ_CLASS_CAR);
   EXPECT_FLOAT_EQ(fbk_object_data.class_prob_pedestrian, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.class_prob_2wheel, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.class_prob_car, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.class_prob_truck, FBK_ONE_F);
   EXPECT_EQ(fbk_object_data.id_merged_obj, FBK_ONE_INT);
   EXPECT_TRUE(fbk_object_data.f_merge_occured);
   EXPECT_EQ(fbk_object_data.curvi_coordinates_calc_method, PA_OBJ_CURVI_COORDINATES_SNAIL_TRAIL);
   EXPECT_FLOAT_EQ(fbk_object_data.curvi_pos.x, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.curvi_pos.y, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.curvi_vel_rel.x, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.curvi_vel.x, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.curvi_vel.y, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.curvi_vel_rel.y, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.curvi_heading, FBK_ONE_F);
   EXPECT_FLOAT_EQ(fbk_object_data.accuracy_heading, FBK_ONE_F);
   EXPECT_TRUE(fbk_object_data.f_is_fl_origin_sensor);
   EXPECT_TRUE(fbk_object_data.f_is_fr_origin_sensor);
   EXPECT_TRUE(fbk_object_data.f_is_rl_origin_sensor);
   EXPECT_TRUE(fbk_object_data.f_is_rr_origin_sensor);
   EXPECT_TRUE(fbk_object_data.f_is_in_fl_sensor_fov);
   EXPECT_TRUE(fbk_object_data.f_is_in_fr_sensor_fov);
   EXPECT_TRUE(fbk_object_data.f_is_in_rl_sensor_fov);
   EXPECT_TRUE(fbk_object_data.f_is_in_rr_sensor_fov);
   EXPECT_TRUE(fbk_object_data.f_stationary);
   EXPECT_TRUE(fbk_object_data.f_moveable);
   EXPECT_FALSE(fbk_object_data.f_stationary_clutter);
}

/**
 * Set up an object and test whether its state is valid. Here it is mature thus its valid.
 * \uts{CSCSA-42435} \sdd{SF-4177} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Is_Obj_State_Valid__object_is_mature)
{
   /** \arrange set up an old and wise object. */
   boolean_T res;
   uint8_t obj_index                             = FBK_ZERO_UINT;
   context.p_data->object_data[obj_index].age    = 255u;
   context.p_data->object_data[obj_index].status = PA_OBJ_STATUS_MATURE;

   /** \action executes function to test. */
   res = Fbk_Is_Obj_State_Valid(object_data[obj_index].status);

   /** \assert expect that object is valid. */
   EXPECT_TRUE(res);
}


/**
 * Set up an object and test whether its state is valid. Here it is mature thus its valid.
 * \uts{CSCSA-42436} \sdd{SF-4177} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Is_Obj_State_Valid__object_is_coasting)
{
   /** \arrange set up an old and wise object. */
   boolean_T res;
   uint8_t obj_index                             = FBK_ZERO_UINT;
   context.p_data->object_data[obj_index].age    = 255u;
   context.p_data->object_data[obj_index].status = PA_OBJ_STATUS_COASTED;

   /** \action executes function to test. */
   res = Fbk_Is_Obj_State_Valid(object_data[obj_index].status);

   /** \assert expect that object is valid. */
   EXPECT_TRUE(res);
}


/**
 * Set up an object and test whether its state is valid. Here it is invalid thus its not valid.
 * \uts{CSCSA-42437} \sdd{SF-4177} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Is_Obj_State_Valid__object_is_invalid)
{
   /** \arrange set up an old and wise object. */
   boolean_T res;
   uint8_t obj_index                             = FBK_ZERO_UINT;
   context.p_data->object_data[obj_index].age    = 0u;
   context.p_data->object_data[obj_index].status = PA_OBJ_STATUS_INVALID;

   /** \action executes function to test. */
   res = Fbk_Is_Obj_State_Valid(object_data[obj_index].status);

   /** \assert expect that object is invalid. */
   EXPECT_FALSE(res);
}

/**
 * Provide a negative lateral position and call Fbk_Get_Obj_Side. Check that the estimated side equals FBK_SIDE_LEFT.
 * \uts{CSCSA-42438} \sdd{CSCSA-31267} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Get_Obj_Side__returns_left_side_when_lateral_position_is_negative)
{
   /** \arrange Set up lateral position. */
   float32_T lat_pos = -1.5f;
   uint8_t result_side;

   /** \action Call Fbk_Get_Obj_Side to estimate the side corresponding to the lateral position. */
   result_side = Fbk_Get_Obj_Side(lat_pos);

   /** \assert Check that estimated side equals FBK_SIDE_LEFT. */
   EXPECT_EQ(result_side, FBK_SIDE_LEFT);
}

/**
 * Provide a positive lateral position and call Fbk_Get_Obj_Side. Check that the estimated side equals FBK_SIDE_RIGHT.
 * \uts{CSCSA-42439} \sdd{CSCSA-31267} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Get_Obj_Side__returns_right_side_when_lateral_position_is_positive)
{
   /** \arrange Set up lateral position. */
   float32_T lat_pos = 2.5f;
   uint8_t result_side;

   /** \action Call Fbk_Get_Obj_Side to estimate the side corresponding to the lateral position. */
   result_side = Fbk_Get_Obj_Side(lat_pos);

   /** \assert Check that estimated side equals FBK_SIDE_RIGHT. */
   EXPECT_EQ(result_side, FBK_SIDE_RIGHT);
}

/**
 * Provide zero as lateral position and call Fbk_Get_Obj_Side. Check that the estimated side equals FBK_SIDE_RIGHT.
 * \uts{CSCSA-42440} \sdd{CSCSA-31267} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Get_Obj_Side__returns_right_side_when_lateral_position_is_zero)
{
   /** \arrange Set up lateral position. */
   float32_T lat_pos = 0.0f;
   uint8_t result_side;

   /** \action Call Fbk_Get_Obj_Side to estimate the side corresponding to the lateral position. */
   result_side = Fbk_Get_Obj_Side(lat_pos);

   /** \assert Check that estimated side equals FBK_SIDE_RIGHT. */
   EXPECT_EQ(result_side, FBK_SIDE_RIGHT);
}

/*
 * Tests if an object with negative lateral curvi position is detected as being on the left side.
 * \uts{CSCSA-42441} \sdd{CSCSA-31268} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Get_Obj_Side_Coord_Sys__left_side_curvi)
{
   /** \arrange Create target which at left side of the ego. */
   fbk_object_data.curvi_pos.y = -5.0f;

   /** \action Set object criticallity */
   uint8_t side = Fbk_Get_Obj_Side_Coord_Sys(&fbk_object_data, FBK_TRUE);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(side, FBK_SIDE_LEFT);
}

/*
 * Tests if an object with positive lateral curvi position is detected as being on the right side.
 * \uts{CSCSA-42442} \sdd{CSCSA-31268} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Get_Obj_Side_Coord_Sys__right_side_curvi)
{
   /** \arrange Create target which at right side of the ego. */
   fbk_object_data.curvi_pos.y = 5.0f;

   /** \action Set object criticallity */
   uint8_t side = Fbk_Get_Obj_Side_Coord_Sys(&fbk_object_data, FBK_TRUE);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(side, FBK_SIDE_RIGHT);
}

/*
 * Tests if an object with negative lateral vcs position is detected as being on the left side.
 * \uts{CSCSA-42443} \sdd{CSCSA-31268} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Get_Obj_Side_Coord_Sys__left_side_vcs)
{
   /** \arrange Create target which at left side of the ego. */
   fbk_object_data.vcs_pos.y = -5.0f;

   /** \action Set object criticallity */
   uint8_t side = Fbk_Get_Obj_Side_Coord_Sys(&fbk_object_data, FBK_FALSE);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(side, FBK_SIDE_LEFT);
}

/*
 * Tests if an object with positive lateral vcs position is detected as being on the right side.
 * \uts{CSCSA-42444} \sdd{CSCSA-31268} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Get_Obj_Side_Coord_Sys__right_side_vcs)
{
   /** \arrange Create target which at right side of the ego. */
   fbk_object_data.vcs_pos.y = 5.0f;

   /** \action Set object criticallity */
   uint8_t side = Fbk_Get_Obj_Side_Coord_Sys(&fbk_object_data, FBK_FALSE);

   /** \assert Verify object criticallity is filled correctly. */
   EXPECT_EQ(side, FBK_SIDE_RIGHT);
}

/**
 * Provide a negative lateral position and call Fbk_Get_Obj_Side_Sign. Check that the side sign equals -1.
 * \uts{CSCSA-42445} \sdd{CSCSA-31269} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Get_Obj_Side_Sign__returns_minus_when_lateral_position_is_negative)
{
   /** \arrange Set up lateral position. */
   float32_T lat_pos = -1.5f;
   float32_T sign;

   /** \action Call Fbk_Get_Obj_Side_Sign to return sign of the side corresponding to the lateral position. */
   sign = Fbk_Get_Obj_Side_Sign(lat_pos);

   /** \assert Check that side sign equals -1.0f. */
   EXPECT_FLOAT_EQ(sign, -1.0f);
}

/**
 * Provide a positive lateral position and call Fbk_Get_Obj_Side_Sign. Check that the side sign equals 1.
 * \uts{CSCSA-42446} \sdd{CSCSA-31269} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Get_Obj_Side_Sign__returns_plus_when_lateral_position_is_positive)
{
   /** \arrange Set up lateral position. */
   float32_T lat_pos = 2.5f;
   float32_T sign;

   /** \action Call Fbk_Get_Obj_Side_Sign to return sign of the side corresponding to the lateral position. */
   sign = Fbk_Get_Obj_Side_Sign(lat_pos);

   /** \assert Check that side sign equals 1.0f. */
   EXPECT_FLOAT_EQ(sign, FBK_ONE_F);
}

/**
 * Provide zero as lateral position and call Fbk_Get_Obj_Side_Sign. Check that the side sign equals 1.
 * \uts{CSCSA-42447} \sdd{CSCSA-31269} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Get_Obj_Side_Sign__returns_plus_when_lateral_position_is_zero)
{
   /** \arrange Set up lateral position. */
   float32_T lat_pos = 0.0f;
   float32_T sign;

   /** \action Call Fbk_Get_Obj_Side_Sign to return sign of the side corresponding to the lateral position. */
   sign = Fbk_Get_Obj_Side_Sign(lat_pos);

   /** \assert Check that side sign equals 1.0f. */
   EXPECT_FLOAT_EQ(sign, FBK_ONE_F);
}

/**
 * Provide FBK_SIDE_LEFT as object side and call Fbk_Convert_Obj_Side_To_Sign. Check that the side sign equals -1.
 * \uts{CSCSA-42448} \sdd{CSCSA-31270} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Convert_Obj_Side_To_Sign__returns_minus_when_side_left)
{
   /** \arrange Set up object side. */
   uint8_t side = FBK_SIDE_LEFT;
   float32_T sign;

   /** \action Fbk_Convert_Obj_Side_To_Sign to return sign of the object depending of its side assigned. */
   sign = Fbk_Convert_Obj_Side_To_Sign(side);

   /** \assert Check that side sign equals -1.0f. */
   EXPECT_FLOAT_EQ(sign, -1.0f);
}

/**
 * Provide FBK_SIDE_RIGHT as object side and call Fbk_Convert_Obj_Side_To_Sign. Check that the side sign equals 1.
 * \uts{CSCSA-42449} \sdd{CSCSA-31270} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Convert_Obj_Side_To_Sign__returns_plus_when_side_right)
{
   /** \arrange Set up object side. */
   uint8_t side = FBK_SIDE_RIGHT;
   float32_T sign;

   /** \action Fbk_Convert_Obj_Side_To_Sign to return sign of the object depending of its side assigned. */
   sign = Fbk_Convert_Obj_Side_To_Sign(side);

   /** \assert Check that side sign equals 1.0f. */
   EXPECT_FLOAT_EQ(sign, FBK_ONE_F);
}

/**
 * Test case to verify that Fbk_Verify_Object_Data_Range returns true when all object data is within valid ranges.
 * \uts{CSCSA-308170} \sdd{CSCSA-307402} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Verify_Object_Data_Range__returns_true_for_valid_data)
{
   /** \arrange Set up object data with valid ranges. */
   fbk_object_data.id                            = 34u;
   fbk_object_data.status                        = PA_OBJ_STATUS_MATURE;
   fbk_object_data.age                           = FBK_ONE_UINT;
   fbk_object_data.stage_age                     = FBK_ONE_UINT;
   fbk_object_data.fbk_stage_age                 = FBK_ONE_UINT;
   fbk_object_data.existence_probability         = FBK_ONE_F;
   fbk_object_data.vcs_pos.x                     = FBK_ONE_F;
   fbk_object_data.vcs_vel.x                     = FBK_ONE_F;
   fbk_object_data.vcs_vel_rel.x                 = FBK_ONE_F;
   fbk_object_data.vcs_accel.x                   = FBK_ONE_F;
   fbk_object_data.vcs_pos.y                     = FBK_ONE_F;
   fbk_object_data.vcs_vel.y                     = FBK_ONE_F;
   fbk_object_data.vcs_vel_rel.y                 = FBK_ONE_F;
   fbk_object_data.vcs_accel.y                   = FBK_ONE_F;
   fbk_object_data.vcs_heading                   = FBK_ONE_F;
   fbk_object_data.heading_rate                  = FBK_ONE_F;
   fbk_object_data.heading_variance              = FBK_ONE_F;
   fbk_object_data.speed                         = FBK_ONE_F;
   fbk_object_data.eclipse_value                 = FBK_ONE_F;
   fbk_object_data.length                        = FBK_ONE_F;
   fbk_object_data.width                         = FBK_ONE_F;
   fbk_object_data.obj_distance                  = FBK_ONE_F;
   fbk_object_data.obstruction_prob              = FBK_ONE_F;
   fbk_object_data.f_reflection                  = FBK_TRUE;
   fbk_object_data.obj_class                     = PA_OBJ_CLASS_CAR;
   fbk_object_data.class_prob_pedestrian         = FBK_ONE_F;
   fbk_object_data.class_prob_2wheel             = FBK_ONE_F;
   fbk_object_data.class_prob_car                = FBK_ONE_F;
   fbk_object_data.class_prob_truck              = FBK_ONE_F;
   fbk_object_data.id_merged_obj                 = FBK_ONE_INT;
   fbk_object_data.f_merge_occured               = FBK_TRUE;
   fbk_object_data.curvi_coordinates_calc_method = PA_OBJ_CURVI_COORDINATES_SNAIL_TRAIL;
   fbk_object_data.curvi_pos.x                   = FBK_ONE_F;
   fbk_object_data.curvi_vel.x                   = FBK_ONE_F;
   fbk_object_data.curvi_vel_rel.x               = FBK_ONE_F;
   fbk_object_data.curvi_pos.y                   = FBK_ONE_F;
   fbk_object_data.curvi_vel.y                   = FBK_ONE_F;
   fbk_object_data.curvi_vel_rel.y               = FBK_ONE_F;
   fbk_object_data.curvi_heading                 = FBK_ONE_F;
   fbk_object_data.accuracy_heading              = FBK_ONE_F;
   fbk_object_data.f_is_fl_origin_sensor         = FBK_TRUE;
   fbk_object_data.f_is_fr_origin_sensor         = FBK_TRUE;
   fbk_object_data.f_is_rl_origin_sensor         = FBK_TRUE;
   fbk_object_data.f_is_rr_origin_sensor         = FBK_TRUE;
   fbk_object_data.f_is_in_fl_sensor_fov         = FBK_TRUE;
   fbk_object_data.f_is_in_fr_sensor_fov         = FBK_TRUE;
   fbk_object_data.f_is_in_rl_sensor_fov         = FBK_TRUE;
   fbk_object_data.f_is_in_rr_sensor_fov         = FBK_TRUE;
   fbk_object_data.f_stationary                  = FBK_TRUE;
   fbk_object_data.f_moveable                    = FBK_TRUE;
   fbk_object_data.f_stationary_clutter          = FBK_TRUE;

   /** \action Call Fbk_Verify_Object_Data_Range with valid object data. */
   boolean_T result = Fbk_Verify_Object_Data_Range(&fbk_object_data);

   /** \assert Check that the function returns true for valid data. */
   EXPECT_TRUE(result);

} /**
   * Test case to verify that Fbk_Verify_Object_Data_Range returns true when all object data is above valid ranges.
   * \uts{CSCSA-308171} \sdd{CSCSA-307402} \testtype{SoftwareUpdateTesting}
   */
TEST_F(Fbk_Object_Validation_Test, Fbk_Verify_Object_Data_Range__returns_false_for_invalid_data_above_threshold)
{
   /** \arrange Set up object data with valid ranges. */
   fbk_object_data.id                            = 255u;
   fbk_object_data.status                        = (Pa_Obj_Status_T) 5u;
   fbk_object_data.age                           = FBK_ONE_UINT;
   fbk_object_data.stage_age                     = FBK_ONE_UINT;
   fbk_object_data.fbk_stage_age                 = FBK_ONE_UINT;
   fbk_object_data.existence_probability         = FBK_EXISTENCE_PROBABILITY_MAX_VAL + EPSILON;
   fbk_object_data.vcs_pos.x                     = FBK_VCS_POS_X_MAX_VAL + EPSILON;
   fbk_object_data.vcs_vel.x                     = FBK_VCS_VEL_X_MAX_VAL + EPSILON;
   fbk_object_data.vcs_vel_rel.x                 = FBK_VCS_VEL_REL_X_MAX_VAL + EPSILON;
   fbk_object_data.vcs_accel.x                   = FBK_VCS_ACCEL_X_MAX_VAL + EPSILON;
   fbk_object_data.vcs_pos.y                     = FBK_VCS_POS_Y_MAX_VAL + EPSILON;
   fbk_object_data.vcs_vel.y                     = FBK_VCS_VEL_Y_MAX_VAL + EPSILON;
   fbk_object_data.vcs_vel_rel.y                 = FBK_VCS_VEL_REL_Y_MAX_VAL + EPSILON;
   fbk_object_data.vcs_accel.y                   = FBK_VCS_ACCEL_Y_MAX_VAL + EPSILON;
   fbk_object_data.vcs_heading                   = FBK_VCS_HEADING_MAX_VAL + EPSILON;
   fbk_object_data.heading_rate                  = FBK_HEADING_RATE_MAX_VAL + EPSILON;
   fbk_object_data.heading_variance              = FBK_HEADING_VARIANCE_MAX_VAL + EPSILON;
   fbk_object_data.speed                         = FBK_SPEED_MAX_VAL + EPSILON;
   fbk_object_data.eclipse_value                 = FBK_ECLIPSE_VALUE_MAX_VAL + EPSILON;
   fbk_object_data.length                        = FBK_LENGTH_MAX_VAL + EPSILON;
   fbk_object_data.width                         = FBK_WIDTH_MAX_VAL + EPSILON;
   fbk_object_data.obj_distance                  = FBK_OBJ_DISTANCE_MAX_VAL + EPSILON;
   fbk_object_data.obstruction_prob              = FBK_OBSTRUCTION_PROB_MAX_VAL + EPSILON;
   fbk_object_data.f_reflection                  = 2u;
   fbk_object_data.obj_class                     = (Pa_Obj_Class_T) 5u;
   fbk_object_data.class_prob_pedestrian         = FBK_CLASS_PROB_PEDESTRIAN_MAX_VAL + EPSILON;
   fbk_object_data.class_prob_2wheel             = FBK_CLASS_PROB_2WHEEL_MAX_VAL + EPSILON;
   fbk_object_data.class_prob_car                = FBK_CLASS_PROB_CAR_MAX_VAL + EPSILON;
   fbk_object_data.class_prob_truck              = FBK_CLASS_PROB_TRUCK_MAX_VAL + EPSILON;
   fbk_object_data.id_merged_obj                 = FBK_ONE_INT;
   fbk_object_data.f_merge_occured               = 2u;
   fbk_object_data.curvi_coordinates_calc_method = (Pa_Obj_Curvi_Calc_Method_T) 4u;
   fbk_object_data.curvi_pos.x                   = FBK_CURVI_POS_X_MAX_VAL + EPSILON;
   fbk_object_data.curvi_vel.x                   = FBK_CURVI_VEL_X_MAX_VAL + EPSILON;
   fbk_object_data.curvi_vel_rel.x               = FBK_CURVI_VEL_REL_X_MAX_VAL + EPSILON;
   fbk_object_data.curvi_pos.y                   = FBK_CURVI_POS_Y_MAX_VAL + EPSILON;
   fbk_object_data.curvi_vel.y                   = FBK_CURVI_VEL_Y_MAX_VAL + EPSILON;
   fbk_object_data.curvi_vel_rel.y               = FBK_CURVI_VEL_REL_Y_MAX_VAL + EPSILON;
   fbk_object_data.curvi_heading                 = FBK_CURVI_HEADING_MAX_VAL + EPSILON;
   fbk_object_data.accuracy_heading              = FBK_ACCURACY_HEADING_MAX_VAL + EPSILON;
   fbk_object_data.f_is_fl_origin_sensor         = 2u;
   fbk_object_data.f_is_fr_origin_sensor         = 2u;
   fbk_object_data.f_is_rl_origin_sensor         = 2u;
   fbk_object_data.f_is_rr_origin_sensor         = 2u;
   fbk_object_data.f_is_in_fl_sensor_fov         = 2u;
   fbk_object_data.f_is_in_fr_sensor_fov         = 2u;
   fbk_object_data.f_is_in_rl_sensor_fov         = 2u;
   fbk_object_data.f_is_in_rr_sensor_fov         = 2u;
   fbk_object_data.f_stationary                  = 2u;
   fbk_object_data.f_moveable                    = 2u;
   fbk_object_data.f_stationary_clutter          = 2u;

   /** \action Call Fbk_Verify_Object_Data_Range with valid object data. */
   boolean_T result = Fbk_Verify_Object_Data_Range(&fbk_object_data);

   /** \assert Check that the function returns false for invalid data. */
   EXPECT_FALSE(result);
}

/**
 * Test case to verify that Fbk_Verify_Object_Data_Range returns true when all object data is below valid ranges.
 * \uts{CSCSA-308172} \sdd{CSCSA-307402} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Object_Validation_Test, Fbk_Verify_Object_Data_Range__returns_false_for_invalid_data_below_threshold)
{
   /** \arrange Set up object data with valid ranges. */
   fbk_object_data.id                            = 255u;
   fbk_object_data.status                        = (Pa_Obj_Status_T) 5u;
   fbk_object_data.age                           = FBK_ONE_UINT;
   fbk_object_data.stage_age                     = FBK_ONE_UINT;
   fbk_object_data.fbk_stage_age                 = FBK_ONE_UINT;
   fbk_object_data.existence_probability         = FBK_EXISTENCE_PROBABILITY_MIN_VAL - EPSILON;
   fbk_object_data.vcs_pos.x                     = FBK_VCS_POS_X_MIN_VAL - EPSILON;
   fbk_object_data.vcs_vel.x                     = FBK_VCS_VEL_X_MIN_VAL - EPSILON;
   fbk_object_data.vcs_vel_rel.x                 = FBK_VCS_VEL_REL_X_MIN_VAL - EPSILON;
   fbk_object_data.vcs_accel.x                   = FBK_VCS_ACCEL_X_MIN_VAL - EPSILON;
   fbk_object_data.vcs_pos.y                     = FBK_VCS_POS_Y_MIN_VAL - EPSILON;
   fbk_object_data.vcs_vel.y                     = FBK_VCS_VEL_Y_MIN_VAL - EPSILON;
   fbk_object_data.vcs_vel_rel.y                 = FBK_VCS_VEL_REL_Y_MIN_VAL - EPSILON;
   fbk_object_data.vcs_accel.y                   = FBK_VCS_ACCEL_Y_MIN_VAL - EPSILON;
   fbk_object_data.vcs_heading                   = FBK_VCS_HEADING_MIN_VAL - EPSILON;
   fbk_object_data.heading_rate                  = FBK_HEADING_RATE_MIN_VAL - EPSILON;
   fbk_object_data.heading_variance              = FBK_HEADING_VARIANCE_MIN_VAL - EPSILON;
   fbk_object_data.speed                         = FBK_SPEED_MIN_VAL - EPSILON;
   fbk_object_data.eclipse_value                 = FBK_ECLIPSE_VALUE_MIN_VAL - EPSILON;
   fbk_object_data.length                        = FBK_LENGTH_MIN_VAL - EPSILON;
   fbk_object_data.width                         = FBK_WIDTH_MIN_VAL - EPSILON;
   fbk_object_data.obj_distance                  = FBK_OBJ_DISTANCE_MIN_VAL - EPSILON;
   fbk_object_data.obstruction_prob              = FBK_OBSTRUCTION_PROB_MIN_VAL - EPSILON;
   fbk_object_data.f_reflection                  = FBK_FALSE;
   fbk_object_data.obj_class                     = (Pa_Obj_Class_T) 0u;
   fbk_object_data.class_prob_pedestrian         = FBK_CLASS_PROB_PEDESTRIAN_MIN_VAL - EPSILON;
   fbk_object_data.class_prob_2wheel             = FBK_CLASS_PROB_2WHEEL_MIN_VAL - EPSILON;
   fbk_object_data.class_prob_car                = FBK_CLASS_PROB_CAR_MIN_VAL - EPSILON;
   fbk_object_data.class_prob_truck              = FBK_CLASS_PROB_TRUCK_MIN_VAL - EPSILON;
   fbk_object_data.id_merged_obj                 = FBK_ONE_INT;
   fbk_object_data.f_merge_occured               = FBK_FALSE;
   fbk_object_data.curvi_coordinates_calc_method = (Pa_Obj_Curvi_Calc_Method_T) 0u;
   fbk_object_data.curvi_pos.x                   = FBK_CURVI_POS_X_MIN_VAL - EPSILON;
   fbk_object_data.curvi_vel.x                   = FBK_CURVI_VEL_X_MIN_VAL - EPSILON;
   fbk_object_data.curvi_vel_rel.x               = FBK_CURVI_VEL_REL_X_MIN_VAL - EPSILON;
   fbk_object_data.curvi_pos.y                   = FBK_CURVI_POS_Y_MIN_VAL - EPSILON;
   fbk_object_data.curvi_vel.y                   = FBK_CURVI_VEL_Y_MIN_VAL - EPSILON;
   fbk_object_data.curvi_vel_rel.y               = FBK_CURVI_VEL_REL_Y_MIN_VAL - EPSILON;
   fbk_object_data.curvi_heading                 = FBK_CURVI_HEADING_MIN_VAL - EPSILON;
   fbk_object_data.accuracy_heading              = FBK_ACCURACY_HEADING_MIN_VAL - EPSILON;
   fbk_object_data.f_is_fl_origin_sensor         = FBK_FALSE;
   fbk_object_data.f_is_fr_origin_sensor         = FBK_FALSE;
   fbk_object_data.f_is_rl_origin_sensor         = FBK_FALSE;
   fbk_object_data.f_is_rr_origin_sensor         = FBK_FALSE;
   fbk_object_data.f_is_in_fl_sensor_fov         = FBK_FALSE;
   fbk_object_data.f_is_in_fr_sensor_fov         = FBK_FALSE;
   fbk_object_data.f_is_in_rl_sensor_fov         = FBK_FALSE;
   fbk_object_data.f_is_in_rr_sensor_fov         = FBK_FALSE;
   fbk_object_data.f_stationary                  = FBK_FALSE;
   fbk_object_data.f_moveable                    = FBK_FALSE;
   fbk_object_data.f_stationary_clutter          = FBK_FALSE;

   /** \action Call Fbk_Verify_Object_Data_Range with valid object data. */
   boolean_T result = Fbk_Verify_Object_Data_Range(&fbk_object_data);

   /** \assert Check that the function returns false for invalid data. */
   EXPECT_FALSE(result);
}