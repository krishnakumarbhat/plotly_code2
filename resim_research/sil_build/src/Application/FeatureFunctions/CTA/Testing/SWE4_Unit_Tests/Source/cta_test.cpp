/**
 * @file cta_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for cta.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42043}
 */

#include "cta_test.hpp"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <limits.h>

extern "C"
{
#include "cta.c"
#include "cta_common_functions.h"
#include "fbk_macros.h"
#include "ml_math.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d.h"
#include "pa_shared_types.h"
}


/**
 * Checks if reset function is called internally, when speed is below upper limit
 * \uts{CSCSA-42137} \sdd{SF-3720} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Core_Run__runs_if_all_conditions_are_fulfilled)
{
   /** \arrange Set f_cta_switch to true and host speed below threshold. */
   core_input.f_cta_switch    = FBK_TRUE;
   p_vehicle_data->host_speed = p_cals->k_cta_ego_abs_speed_max - EPSILON;

   data.object_data->id     = 2;
   data.object_data->status = PA_OBJ_STATUS_MATURE;

   cals.k_cta_enable_modes[1] = FBK_FALSE;

   /** \action Call function Cta_Core_Run */
   Cta_Core_Run(&cta_instance);

   /** \assert Verify that CTA status is set to active. */
   EXPECT_EQ(core_output.cta_status, CTA_STATUS_ACTIVE);
}

/**
 * Checks if reset function is called internally, when speed is above upper limit
 * \uts{CSCSA-306961} \sdd{SF-3720} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Core_Run__reset_when_condition_not_fulfilled)
{
   /** \arrange Set f_cta_switch to true and host speed below threshold. */
   core_input.f_cta_switch    = FBK_TRUE;
   p_vehicle_data->host_speed = p_cals->k_cta_ego_abs_speed_max + EPSILON;

   data.object_data->id     = 2;
   data.object_data->status = PA_OBJ_STATUS_MATURE;

   cals.k_cta_enable_modes[1] = FBK_FALSE;

   /** \action Call function Cta_Core_Run */
   Cta_Core_Run(&cta_instance);

   /** \assert Verify that CTA status is set to active. */
   EXPECT_EQ(core_output.cta_status, CTA_STATUS_DEACTIVATED_EGO_SPEED);
}


/**
 * Checks if CTA active status is set correctly when all conditions apply.
 * \uts{CSCSA-42138} \sdd{SF-4056} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Status__sets_active_status_correctly)
{
   /** \arrange Set f_cta_switch to true and host speed below threshold. */
   core_input.f_cta_switch    = FBK_TRUE;
   p_vehicle_data->host_speed = -(p_cals->k_cta_ego_abs_speed_max - EPSILON);

   /** \action Call function Cta_Get_Status */
   Cta_Status_T cta_status = Cta_Get_Status(p_vehicle_data, p_cals, &core_input);

   /** \assert Verify that CTA status is set to active. */
   EXPECT_EQ(cta_status, CTA_STATUS_ACTIVE);
}

/**
 * Checks if CTA status is set to deactived by host speed, when speed is above upper limit
 * \uts{CSCSA-42139} \sdd{SF-4056} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Status__sets_deactived_by_host_speed_status_correctly)
{
   /** \arrange Set f_cta_switch to true and host speed above threshold. */
   core_input.f_cta_switch    = FBK_TRUE;
   p_vehicle_data->host_speed = p_cals->k_cta_ego_abs_speed_max + EPSILON;

   /** \action Call function Cta_Get_Status */
   Cta_Status_T cta_status = Cta_Get_Status(p_vehicle_data, p_cals, &core_input);

   /** \assert Verify that CTA status is set to deactivated by host speed. */
   EXPECT_EQ(cta_status, CTA_STATUS_DEACTIVATED_EGO_SPEED);
}

/**
 * Checks if CTA disabled status is set correctly when feature is disabled by calibrations.
 * \uts{CSCSA-42140} \sdd{SF-4056} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Status__sets_disabled_status_correctly)
{
   /** \arrange Set f_cta_switch to false and host speed below threshold. */
   core_input.f_cta_switch    = FBK_FALSE;
   p_vehicle_data->host_speed = p_cals->k_cta_ego_abs_speed_max - EPSILON;

   /** \action Call function Cta_Get_Status */
   Cta_Status_T cta_status = Cta_Get_Status(p_vehicle_data, p_cals, &core_input);

   /** \assert Verify that CTA status is set to disabled. */
   EXPECT_EQ(cta_status, CTA_STATUS_DISABLED);
}

/**
 * Tests funtionality of longitudinal intersection point calculation Heading is set to 0 and thus the intersection point shall lie
 * in infinity since the object is moving perpendicular to the lateral axis. \uts{CSCSA-42141} \sdd{SF-3707}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Longitudinal_Intersection__use_k_cta_f_use_relative_velocity_for_intersect_point_OFF)
{
   /** \arrange object pointing parallel to longitudinal axis */
   p_cals->k_cta_f_use_rel_vel_isect_point_calc                  = FBK_FALSE;
   cals.k_cta_f_apply_heading_compensation_on_intersection_point = FBK_FALSE;

   object.attributes->CTA_heading                                = 0.0f;
   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point.x = 0.0f;
   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point.y = 0.0f;
   object.attributes->relative_velocity.x                        = 1.0f;
   object.attributes->relative_velocity.y                        = 0.0f;
   object.attributes->ttc                                        = 6.0f;

   /** \action executes function to test */
   Cta_Get_Longitudinal_Intersection(&object, p_cals, p_vehicle_data, mode);

   /** \assert expect longitudinal intersection point to lie in infinity */
   EXPECT_EQ(object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode], CTA_HIGH_DEFAULT_VAL);
}

/**
 * Tests funtionality of longitudinal intersection point calculation The relative velocity shall be used for the intersection point
 * calculation at that point and thus the cta_heading shall not have any effect here. \uts{CSCSA-42142} \sdd{SF-3707}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Longitudinal_Intersection__use_k_cta_f_use_relative_velocity_for_intersect_point_ON)
{
   /** \arrange object for which the intersection point shall be calculated with relative velocity */
   float32_T expected_val;
   p_cals->k_cta_f_use_rel_vel_isect_point_calc                  = FBK_TRUE;
   cals.k_cta_f_apply_heading_compensation_on_intersection_point = FBK_FALSE;
   object.attributes->CTA_heading                                = 0.0f;
   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point.x = 0.0f;
   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point.y = 0.0f;
   object.attributes->relative_velocity.x                        = 1.0f;
   object.attributes->relative_velocity.y                        = 0.0f;

   object.attributes->ttc = 6.0f;

   expected_val = object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point.x
                  + (object.attributes->ttc * object.attributes->relative_velocity.x);

   /** \action executes function to test */
   Cta_Get_Longitudinal_Intersection(&object, p_cals, p_vehicle_data, mode);

   /** \assert expect equality between precalculated result and returned value */
   EXPECT_EQ(object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode], expected_val);
}

#ifndef NDEBUG
/**
 * Tests funtionality of longitudinal intersection point calculation Here an assertion is expected, since object is pointing to
 * NULL. \uts{CSCSA-42143} \sdd{SF-3707} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Longitudinal_Intersection__object_NULL)
{
   EXPECT_DEATH(
      {
         /** \arrange object pointing to NULL */
         /** \action executes function to test */
         Cta_Get_Longitudinal_Intersection(NULL, p_cals, p_vehicle_data, mode);
         /** \assert assertion since object points to NULL */
      },
      ".*p_object.*");
}

/**
 * Tests funtionality of longitudinal intersection point calculation Here an assertion is expected, since calibration is pointing
 * to NULL. \uts{CSCSA-42144} \sdd{SF-3707} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Longitudinal_Intersection__cals_NULL)
{
   EXPECT_DEATH(
      {
         /** \arrange calibrations pointing to NULL */
         /** \action executes function to test */
         Cta_Get_Longitudinal_Intersection(&object, NULL, p_vehicle_data, mode);
         /** \assert assertion since calc point to NULL */
      },
      ".*p_cta_cal.*");
}
#endif

/**
 * Tests funtionality of longitudinal intersection point calculation Here the object is pointing parallel to lateral axis from the
 * right side to the longitudinal axis. \uts{CSCSA-42145} \sdd{SF-3707} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Longitudinal_Intersection__heading_90)
{
   /** \arrange object pointing to the right */
   p_cals->k_cta_f_use_rel_vel_isect_point_calc                     = FBK_FALSE;
   p_cals->k_cta_f_apply_heading_compensation_on_intersection_point = FBK_TRUE;
   object.attributes->CTA_heading                                   = 0.5f * PI;
   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point      = Create_2d_Vector_Origin();

   /** \action executes function to test */
   Cta_Get_Longitudinal_Intersection(&object, p_cals, p_vehicle_data, mode);

   /** \assert expect longitudinal intersection in origin */
   EXPECT_NEAR(object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode], 0.0f, EPSILON);
}

/**
 * Tests funtionality of longitudinal intersection point calculation Here the object is pointing parallel to lateral axis from the
 * left side to the longitudinal axis. \uts{CSCSA-42146} \sdd{SF-3707} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Longitudinal_Intersection__heading_270)
{
   /** \arrange object pointing to the right */
   mode                                                             = CTA_MODE_REAR;
   p_cals->k_cta_f_use_rel_vel_isect_point_calc                     = FBK_FALSE;
   p_cals->k_cta_f_apply_heading_compensation_on_intersection_point = FBK_FALSE;
   object.attributes->CTA_heading                                   = 1.5f * PI;
   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point      = Create_2d_Vector_Origin();

   /** \action executes function to test */
   Cta_Get_Longitudinal_Intersection(&object, p_cals, p_vehicle_data, mode);

   /** \assert expect longitudinal intersection in origin */
   EXPECT_EQ(object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode], 0.0f);
}

/**
 * Tests funtionality of longitudinal intersection point calculation Here the object heading is equal PI.
 * \uts{CSCSA-188378} \sdd{SF-3707} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Longitudinal_Intersection__heading_PI)
{
   /** \arrange object pointing to the right */
   mode                                                             = CTA_MODE_FRONT;
   p_cals->k_cta_f_use_rel_vel_isect_point_calc                     = FBK_FALSE;
   p_cals->k_cta_f_apply_heading_compensation_on_intersection_point = FBK_FALSE;
   object.attributes->CTA_heading                                   = PI - EPSILON / 2;
   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point      = Create_2d_Vector_Origin();

   /** \action executes function to test */
   Cta_Get_Longitudinal_Intersection(&object, p_cals, p_vehicle_data, mode);

   /** \assert expect longitudinal intersection in origin */
   EXPECT_EQ(object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode], CTA_HIGH_DEFAULT_VAL);
}

/**
 * Tests funtionality of longitudinal intersection point calculation Here the object heading is equal minus PI.
 * \uts{CSCSA-188379} \sdd{SF-3707} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Longitudinal_Intersection__heading_minus_PI)
{
   /** \arrange object pointing to the right */
   mode                                                             = CTA_MODE_REAR;
   p_cals->k_cta_f_use_rel_vel_isect_point_calc                     = FBK_FALSE;
   p_cals->k_cta_f_apply_heading_compensation_on_intersection_point = FBK_FALSE;
   object.attributes->CTA_heading                                   = -PI;
   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point      = Create_2d_Vector_Origin();

   /** \action executes function to test */
   Cta_Get_Longitudinal_Intersection(&object, p_cals, p_vehicle_data, mode);

   /** \assert expect longitudinal intersection in origin */
   EXPECT_EQ(object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode], CTA_HIGH_DEFAULT_VAL);
}


/**
 * Tests funtionality of longitudinal intersection point calculation Here the object heading is equal zero.
 * \uts{CSCSA-188380} \sdd{SF-3707} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Longitudinal_Intersection__heading_zero)
{
   /** \arrange object pointing to the right */
   mode                                                             = CTA_MODE_REAR;
   p_cals->k_cta_f_use_rel_vel_isect_point_calc                     = FBK_FALSE;
   p_cals->k_cta_f_apply_heading_compensation_on_intersection_point = FBK_FALSE;
   object.attributes->CTA_heading                                   = 0.0;
   object.attributes->ref_point_candidate[FBK_SIDE_LEFT].point      = Create_2d_Vector_Origin();

   /** \action executes function to test */
   Cta_Get_Longitudinal_Intersection(&object, p_cals, p_vehicle_data, mode);

   /** \assert expect longitudinal intersection in origin */
   EXPECT_EQ(object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode], CTA_HIGH_DEFAULT_VAL);
}

/**
 * Tests funtionality of longitudinal intersection point calculation. Here crashline is set on ego side
 * \uts{CSCSA-246657} \sdd{SF-3707} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Longitudinal_Intersection__crashline_on_ego_side)
{
   /** \arrange object pointing to the right */
   mode                                                             = CTA_MODE_REAR;
   p_cals->k_cta_f_use_rel_vel_isect_point_calc                     = FBK_FALSE;
   p_cals->k_cta_f_apply_heading_compensation_on_intersection_point = FBK_FALSE;
   p_cals->k_cta_intersection_line_host_width_percentage            = 1.0f;
   object.attributes->CTA_heading                                   = -0.785f;
   object.attributes->ref_point_candidate[FBK_SIDE_RIGHT].point     = Create_2d_Vector_Origin();
   float32_T res = object.attributes->ref_point_candidate[FBK_SIDE_RIGHT].point.x
                   - ((object.attributes->ref_point_candidate[FBK_SIDE_RIGHT].point.y - (0.5f * p_vehicle_data->host_width))
                      * (Fast_Cos(object.attributes->CTA_heading) / Fast_Sin(object.attributes->CTA_heading)));

   /** \action executes function to test */
   Cta_Get_Longitudinal_Intersection(&object, p_cals, p_vehicle_data, mode);

   /** \assert expect intersection */
   EXPECT_EQ(object.attributes->long_isect_point_candidate[FBK_SIDE_RIGHT][mode], res);
}

/**
 * Checks whether rear corners of the object passed the longitudinal axis Here the object is approaching from the left side and has
 * already passed the longitudinal axis. \uts{CSCSA-42147} \sdd{SF-3705} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Do_Obj_Corners_Cross_Long_Axis__approach_left_side_turn_off_warning)
{
   /** \arrange object approaching from the left side and already passed the longitudinal axis */
   boolean_T result;
   p_cals->k_cta_dist_thres_crit_level_reset      = 0.0f;
   p_cals->k_cta_f_use_front_corners_dist_stop    = 0u;
   object.attributes->approach_side               = FBK_SIDE_LEFT;
   target_corners.points[FBK_REAR_RIGHT_CORNER].y = 0.01f;
   target_corners.points[FBK_REAR_LEFT_CORNER].y  = 0.01f;

   /** \action executes function to test */
   result = Cta_Do_Obj_Corners_Cross_Long_Axis(&object, &target_corners, p_cals, p_vehicle_data);

   /** \assert expect true */
   EXPECT_TRUE(result);
}

/**
 * Checks whether rear corners of the object passed the longitudinal axis Here the object is approaching from the right side and
 * has already passed the longitudinal axis. \uts{CSCSA-42148} \sdd{SF-3705} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Do_Obj_Corners_Cross_Long_Axis__approach_right_side_turn_off_warning)
{
   /** \arrange object approaching from the right side and already passed the longitudinal axis */
   boolean_T result;

   p_cals->k_cta_dist_thres_crit_level_reset      = 0.0f;
   p_cals->k_cta_f_use_front_corners_dist_stop    = 0u;
   object.attributes->approach_side               = FBK_SIDE_RIGHT;
   p_vehicle_data->host_width                     = 2.0f;
   target_corners.points[FBK_REAR_RIGHT_CORNER].y = -0.01f;
   target_corners.points[FBK_REAR_LEFT_CORNER].y  = -0.01f;

   /** \action executes function to test */
   result = Cta_Do_Obj_Corners_Cross_Long_Axis(&object, &target_corners, p_cals, p_vehicle_data);

   /** \assert expect true */
   EXPECT_TRUE(result);
}

/**
 * Checks whether rear corners of the object passed the longitudinal axis. Here the object is approaching from the left side but
 * has not yet crossed the longitudinal axis. \uts{CSCSA-42149} \sdd{SF-3705} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Do_Obj_Corners_Cross_Long_Axis__approach_left_warning_on)
{
   /** \arrange object approaching from the left side */
   boolean_T result;
   p_cals->k_cta_dist_thres_crit_level_reset      = 0.0f;
   p_cals->k_cta_f_use_front_corners_dist_stop    = 0u;
   object.attributes->approach_side               = FBK_SIDE_LEFT;
   target_corners.points[FBK_REAR_RIGHT_CORNER].y = -0.01f;
   target_corners.points[FBK_REAR_LEFT_CORNER].y  = -0.01f;

   /** \action executes function to test */
   result = Cta_Do_Obj_Corners_Cross_Long_Axis(&object, &target_corners, p_cals, p_vehicle_data);

   /** \assert expect false since object has not crossed the long axis */
   EXPECT_FALSE(result);
}

/**
 * Checks whether rear corners of the object passed the longitudinal axis. Here the object is approaching from the right side but
 * has not yet crossed the longitudinal axis. \uts{CSCSA-42150} \sdd{SF-3705} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Do_Obj_Corners_Cross_Long_Axis__approach_right_warning_on)
{
   /** \arrange object approaching from the right side */
   boolean_T result;
   p_cals->k_cta_dist_thres_crit_level_reset      = 0.0f;
   p_cals->k_cta_f_use_front_corners_dist_stop    = 0u;
   object.attributes->approach_side               = FBK_SIDE_RIGHT;
   target_corners.points[FBK_REAR_RIGHT_CORNER].y = 0.01f;
   target_corners.points[FBK_REAR_LEFT_CORNER].y  = 0.01f;

   /** \action executes function to test */
   result = Cta_Do_Obj_Corners_Cross_Long_Axis(&object, &target_corners, p_cals, p_vehicle_data);

   /** \assert expect false since object has not crossed the long axis */
   EXPECT_FALSE(result);
}

/**
 * Checks whether rear corners of the object passed the longitudinal axis. Here the object is approaching from the right side but
 * not both rear corners have crossed the longitudinal axis. \uts{CSCSA-42151} \sdd{SF-3705} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Do_Obj_Corners_Cross_Long_Axis__approach_right_warning_on_since_not_both_corners_passed_thres)
{
   /** \arrange object approaching from the right side not both corners have passed the long axis */
   boolean_T result;
   p_cals->k_cta_dist_thres_crit_level_reset      = 0.0f;
   p_cals->k_cta_f_use_front_corners_dist_stop    = 0u;
   object.attributes->approach_side               = FBK_SIDE_RIGHT;
   target_corners.points[FBK_REAR_LEFT_CORNER].y  = 0.05f;
   target_corners.points[FBK_REAR_RIGHT_CORNER].y = -0.03f;

   /** \action executes function to test */
   result = Cta_Do_Obj_Corners_Cross_Long_Axis(&object, &target_corners, p_cals, p_vehicle_data);

   /** \assert expect false since object has not crossed the long axis */
   EXPECT_FALSE(result);
}

/**
 * Checks whether rear corners of the object passed the longitudinal axis. Here the object is approaching from the left side but
 * not both rear corners have crossed the longitudinal axis. \uts{CSCSA-42152} \sdd{SF-3705} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Do_Obj_Corners_Cross_Long_Axis__approach_left_warning_on_since_not_both_corners_passed_thres)
{
   /** \arrange object approaching from the left side not both corners have passed the long axis */
   boolean_T result;
   p_cals->k_cta_dist_thres_crit_level_reset      = 0.0f;
   p_cals->k_cta_f_use_front_corners_dist_stop    = 0u;
   object.attributes->approach_side               = FBK_SIDE_LEFT;
   target_corners.points[FBK_REAR_LEFT_CORNER].y  = -0.03f;
   target_corners.points[FBK_REAR_RIGHT_CORNER].y = 0.05f;

   /** \action executes function to test */
   result = Cta_Do_Obj_Corners_Cross_Long_Axis(&object, &target_corners, p_cals, p_vehicle_data);

   /** \assert expect false since object has not crossed the long axis */
   EXPECT_FALSE(result);
}

/**
 * Checks whether the false resault is returinng when object has undefined side.
 * \uts{CSCSA-188381} \sdd{SF-3705} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Do_Obj_Corners_Cross_Long_Axis__object_undefined_side)
{
   /** \arrange Set object approaching side as undefined */
   boolean_T result;
   p_cals->k_cta_dist_thres_crit_level_reset      = 0.0f;
   p_cals->k_cta_f_use_front_corners_dist_stop    = 0u;
   object.attributes->approach_side               = FBK_SIDE_UNDEFINED;
   target_corners.points[FBK_REAR_LEFT_CORNER].y  = 0.f;
   target_corners.points[FBK_REAR_RIGHT_CORNER].y = 0.f;

   /** \action executes function to test */
   result = Cta_Do_Obj_Corners_Cross_Long_Axis(&object, &target_corners, p_cals, p_vehicle_data);

   /** \assert expect false since object has undefined side */
   EXPECT_FALSE(result);
}


/**
 * Checks whether front corners of the object passed the longitudinal axis. Here the object is approaching from the left side and
 * both front corners have crossed the longitudinal axis. \uts{CSCSA-42153} \sdd{SF-3705} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Do_Obj_Corners_Cross_Long_Axis__front_corners_have_passed_axis)
{
   /** \arrange object approaching from the left side and both front corners have passed the long axis */
   boolean_T result;
   p_cals->k_cta_dist_thres_crit_level_reset       = 0.0f;
   p_cals->k_cta_f_use_front_corners_dist_stop     = 1u;
   object.attributes->approach_side                = FBK_SIDE_LEFT;
   target_corners.points[FBK_FRONT_RIGHT_CORNER].y = p_cals->k_cta_dist_thres_crit_level_reset + EPSILON;
   target_corners.points[FBK_FRONT_LEFT_CORNER].y  = p_cals->k_cta_dist_thres_crit_level_reset + EPSILON;

   /** \action executes function to test */
   result = Cta_Do_Obj_Corners_Cross_Long_Axis(&object, &target_corners, p_cals, p_vehicle_data);

   /** \assert expect true since object has crossed the long axis */
   EXPECT_TRUE(result);
}

#ifndef NDEBUG
/**
 * Checks whether rear corners of the object passed the longitudinal axis. Expect assertion since object pointer is NULL
 * \uts{CSCSA-42154} \sdd{SF-3705} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Do_Obj_Corners_Cross_Long_Axis__object_NULL)
{
   EXPECT_DEATH(
      {
         /** \arrange object pointer is NULL */
         /** \action executes function to test */
         Cta_Do_Obj_Corners_Cross_Long_Axis(NULL, &target_corners, p_cals, p_vehicle_data);
         /** \assert expect assertion since object pointer is NULL */
      },
      ".*p_object.*");
}

/**
 * Checks whether rear corners of the object passed the longitudinal axis. Expect assertion since target corners pointer is NULL
 * \uts{CSCSA-42155} \sdd{SF-3705} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Do_Obj_Corners_Cross_Long_Axis__target_corners_NULL)
{
   EXPECT_DEATH(
      {
         /** \arrange target_corners pointer is NULL */
         /** \action executes function to test */
         Cta_Do_Obj_Corners_Cross_Long_Axis(&object, NULL, p_cals, p_vehicle_data);
         /** \assert target corners pointer is NULL */
      },
      ".*p_target_corners.*");
}

/**
 * Checks whether rear corners of the object passed the longitudinal axis. Expect assertion since calibration pointer is NULL
 * \uts{CSCSA-42156} \sdd{SF-3705} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Do_Obj_Corners_Cross_Long_Axis__vehicle_data_NULL)
{
   EXPECT_DEATH(
      {
         /** \arrange calibration pointer to NULL */
         /** \action executes function to test */
         Cta_Do_Obj_Corners_Cross_Long_Axis(&object, &target_corners, NULL, p_vehicle_data);
         /** \assert assertion since calibration is NULL */
      },
      ".*p_cta_cal.*");
}
#endif


/**
 * Checks whether an object was created near suppression line. Here the object was created in the previous cycle near the
 * suppression line. Thus true shall be returned. \uts{CSCSA-42157} \sdd{SF-3709} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Check_If_Obj_Created_Near_Suppression_Line__object_was_created_near_suppr_side_in_prev_cycle)
{
   /** \arrange setup persistent condition */
   object.persistent->f_prev_cta_alert_suppress = FBK_TRUE;
   /** \action executes function to test */
   Cta_Check_If_Obj_Created_Near_Suppression_Line(&object, p_vehicle_data, p_cals);
   /** \assert true */
   EXPECT_TRUE(object.persistent->f_prev_cta_alert_suppress);
}

/**
 * Checks whether an object was created near suppression line. Here the object was created near the suppression line in the current
 * cycle. Thus true shall be returned. \uts{CSCSA-42158} \sdd{SF-3709} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Check_If_Obj_Created_Near_Suppression_Line__object_was_created_near_suppr_side_in_current_cycle)
{
   /** \arrange objects age and position condition */
   object.persistent->f_prev_cta_alert_suppress = FBK_FALSE;
   p_vehicle_data->host_width                   = 1.5f;
   object.tracker_data.vcs_pos.y                = 0.5f * p_vehicle_data->host_width - EPSILON;
   object.tracker_data.age                      = p_cals->k_cta_age_for_new_creation_below_long_intersection - 1;
   /** \action executes function to test */
   Cta_Check_If_Obj_Created_Near_Suppression_Line(&object, p_vehicle_data, p_cals);
   /** \assert true */
   EXPECT_TRUE(object.persistent->f_prev_cta_alert_suppress);
}

/**
 * Checks whether an object was created near suppression line. Here the object was created near the suppression line but already
 * has an age exceeding the condition. Thus false shall be returned. \uts{CSCSA-42159} \sdd{SF-3709}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Check_If_Obj_Created_Near_Suppression_Line__object_was_created_near_suppr_side_but_age_condition_not_fulfilled)
{
   /** \arrange age condition to fail */
   object.persistent->f_prev_cta_alert_suppress = FBK_FALSE;
   p_vehicle_data->host_width                   = 1.5f;
   object.tracker_data.vcs_pos.y                = 0.5f * p_vehicle_data->host_width - EPSILON;
   object.tracker_data.age                      = p_cals->k_cta_age_for_new_creation_below_long_intersection + 1;
   /** \action executes function to test */
   Cta_Check_If_Obj_Created_Near_Suppression_Line(&object, p_vehicle_data, p_cals);
   /** \assert false */
   EXPECT_FALSE(object.persistent->f_prev_cta_alert_suppress);
}


/**
 * Checks whether an object was created near suppression line. Here the object was created outside of the suppression line. Thus
 * false shall be returned. \uts{CSCSA-42160} \sdd{SF-3709} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Check_If_Obj_Created_Near_Suppression_Line__object_was_created_outside_of_the_intersection_line)
{
   /** \arrange age condition to fail */
   object.persistent->f_prev_cta_alert_suppress = FBK_FALSE;
   p_vehicle_data->host_width                   = 1.5f;
   object.tracker_data.vcs_pos.y                = 0.5f * p_vehicle_data->host_width + EPSILON;
   object.tracker_data.age                      = p_cals->k_cta_age_for_new_creation_below_long_intersection + 1;
   /** \action executes function to test */
   Cta_Check_If_Obj_Created_Near_Suppression_Line(&object, p_vehicle_data, p_cals);
   /** \assert false */
   EXPECT_FALSE(object.persistent->f_prev_cta_alert_suppress);
}


/**
 * Checks whether a criticality level shall be reset based on objects properties. Expect true, since objects ttc is below the
 * threshold \uts{CSCSA-42161} \sdd{SF-3713} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Shall_Criticality_Level_Be_Reset__suppress_warning_by_ttc)
{
   /** \arrange objects ttc condition */
   boolean_T result;
   p_cals->k_cta_stop_alert_ttc             = 0.2f;
   object.attributes->f_stop_time_below_ths = FBK_TRUE;
   /** \action executes function to test */
   result = Cta_Shall_Criticality_Level_Be_Reset(p_cals, &object, &target_corners, p_vehicle_data);
   /** \assert true */
   EXPECT_TRUE(result);
}

/**
 * Checks whether further criticality level analysis shall be suppressed for the given object. Here the object is not newly created
 * near the longitudinal intersection line. Thus this function shall return false. \uts{CSCSA-42162} \sdd{SF-3713}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Shall_Criticality_Level_Be_Reset__dont_suppress_warning_due_to_new_creation_behind_hosts_side)
{
   /** \arrange Setup objects creation flag */
   boolean_T result;
   p_cals->k_cta_f_enable_thres_crit_level_reset = FBK_FALSE;
   p_cals->k_cta_stop_alert_ttc                  = -20.0f; /*deactivate ttc alert-stop*/
   /** \action executes function to test */
   result = Cta_Shall_Criticality_Level_Be_Reset(p_cals, &object, &target_corners, p_vehicle_data);
   /** \assert false */
   EXPECT_FALSE(result);
}

/**
 * Checks whether a criticality level shall be reset based on objects properties. Expect true, since objects corners passed
 * threshold \uts{CSCSA-42163} \sdd{SF-3713} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Shall_Criticality_Level_Be_Reset__suppress_warning_by_distance_threshold)
{
   /** \arrange object with corners passing the longitudinal axis */
   boolean_T result;
   object.attributes->approach_side                = FBK_SIDE_RIGHT;
   target_corners.points[FBK_REAR_RIGHT_CORNER].y  = -0.01f;
   target_corners.points[FBK_REAR_LEFT_CORNER].y   = -0.01f;
   target_corners.points[FBK_FRONT_RIGHT_CORNER].y = -0.01f;
   target_corners.points[FBK_FRONT_LEFT_CORNER].y  = -0.01f;
   p_cals->k_cta_f_enable_thres_crit_level_reset   = 1;
   p_cals->k_cta_dist_thres_crit_level_reset       = 0.0f;
   /** \action executes function to test */
   result = Cta_Shall_Criticality_Level_Be_Reset(p_cals, &object, &target_corners, p_vehicle_data);
   /** \assert expect a reset of crit level */
   EXPECT_TRUE(result);
}

/**
 * Checks whether a criticality level shall be reset based on objects properties. Expect false because object rear corners do not
 * cross the threshold \uts{CSCSA-188382} \sdd{SF-3713} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Shall_Criticality_Level_Be_Reset__dont_suppress_warning_corners_not_cross_trseshold)
{
   /** \arrange object with corners passing the longitudinal axis */
   boolean_T result;
   object.attributes->approach_side                = FBK_SIDE_LEFT;
   target_corners.points[FBK_REAR_RIGHT_CORNER].y  = -0.01f;
   target_corners.points[FBK_REAR_LEFT_CORNER].y   = -0.01f;
   target_corners.points[FBK_FRONT_RIGHT_CORNER].y = -0.01f;
   target_corners.points[FBK_FRONT_LEFT_CORNER].y  = -0.01f;
   p_cals->k_cta_f_enable_thres_crit_level_reset   = 1;
   p_cals->k_cta_dist_thres_crit_level_reset       = 0.0f;
   /** \action executes function to test */
   result = Cta_Shall_Criticality_Level_Be_Reset(p_cals, &object, &target_corners, p_vehicle_data);
   /** \assert expect a reset of crit level */
   EXPECT_FALSE(result);
}


/**
 * Checks whether a criticality level shall be reset based on objects properties. Expect false, since object does not fulfill any
 * reset condition \uts{CSCSA-42164} \sdd{SF-3713} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Shall_Criticality_Level_Be_Reset__dont_suppress_warning)
{
   /** \arrange */
   boolean_T result;
   p_cals->k_cta_stop_alert_ttc                  = 0.2f;
   object.attributes->ttc                        = 0.21f;
   p_cals->k_cta_f_enable_thres_crit_level_reset = 0;
   /** \action executes function to test */
   result = Cta_Shall_Criticality_Level_Be_Reset(p_cals, &object, &target_corners, p_vehicle_data);
   /** \assert */
   EXPECT_FALSE(result);
}


/**
 * Test Flipping of zone functionality Object is approaching from the right side, so no change of zone is needed
 * \uts{CSCSA-42165} \sdd{SF-3706} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Flip_Zone__approach_from_right_side_dont_change_butterfly)
{
   /** \arrange cta zone */
   Fbk_Field_Of_Interest_T cta_zone{};

   object.attributes->approach_side = FBK_SIDE_RIGHT;

   /** \action executes function to test */
   Cta_Flip_Zone(object.attributes->approach_side, &cta_zone);

   /** \assert expect that the zone is equal to its initialization */
   for (uint8_t i = 0; i < p_cals->k_cta_amount_butterfly_points_in_use; i++)
   {
      EXPECT_FLOAT_EQ(cta_zone.points[i].x, 0.0f);
      EXPECT_FLOAT_EQ(cta_zone.points[i].y, 0.0f);
   }
}

/**
 * Test Flipping of zone functionality Object is approaching from the left side, so the zone is flipped to the left side
 * \uts{CSCSA-42166} \sdd{SF-3706} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Flip_Zone__approach_from_left_side_transform_to_left)
{
   /** \arrange cta zone */
   Fbk_Field_Of_Interest_T cta_zone;

   Cta_Init_Zone(&cta_zone, p_cals);

   object.attributes->approach_side = FBK_SIDE_LEFT;
   core_input.cta_zone.size         = 4;
   /** \action executes function to test */
   Cta_Flip_Zone(object.attributes->approach_side, &cta_zone);

   /** \assert expect the zone points to change their sign */
   for (uint8_t i = 0; i < p_cals->k_cta_amount_butterfly_points_in_use; i++)
   {
      EXPECT_FLOAT_EQ(cta_zone.points[i].y, -p_cals->k_cta_butterfly_lat[i]);
   }
}

/**
 * Tests the relative velocity calculation. Here the cta heading shall be used for calculation of relative velocity. A calculated
 * vector dependend on the path heading is expected. \uts{CSCSA-42171} \sdd{SF-3708} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Relative_Velocity_Of_Object__cta_heading_is_considered_for_relative_velocity_calc)
{
   /** \arrange object with a sample configuration for relative velocity calculation */
   p_cals->k_cta_f_use_heading_for_relative_velocity_calculation = FBK_TRUE;
   object.tracker_data.speed                                     = 5;
   object.attributes->CTA_heading                                = -0.5f * PI;

   float32_T expected_res_x = (object.tracker_data.speed * Fast_Cos(object.attributes->CTA_heading)) - p_vehicle_data->host_speed;
   float32_T expected_res_y = (object.tracker_data.speed * Fast_Sin(object.attributes->CTA_heading));

   /** \action executes function to test */
   Cta_Get_Relative_Velocity_Of_Object(&object, p_vehicle_data, p_cals);

   /** \assert expect equality of returned vector and calculated relative velocity */
   EXPECT_FLOAT_EQ(object.attributes->relative_velocity.x, expected_res_x);
   EXPECT_FLOAT_EQ(object.attributes->relative_velocity.y, expected_res_y);
}


/**
 * Tests the relative velocity calculation.. The relative velocity shall be given by the tracker relative velocity.
 * \uts{CSCSA-42172} \sdd{SF-3708} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Relative_Velocity_Of_Object__trackers_relative_velocity_shall_be_used)
{
   /** \arrange object with a sample configuration for relative velocity calculation */
   p_cals->k_cta_f_use_heading_for_relative_velocity_calculation = FBK_FALSE;
   p_cals->k_cta_speed_thresh_for_rel_vel_calc                   = -1.0f;
   object.tracker_data.vcs_vel_rel.x                             = 5;
   object.tracker_data.vcs_vel_rel.y                             = 3;
   object.attributes->CTA_heading                                = -0.5f * PI;

   /** \action executes function to test */
   Cta_Get_Relative_Velocity_Of_Object(&object, p_vehicle_data, p_cals);

   /** \assert expect equality of returned vector and the trackers relative velocity */
   EXPECT_FLOAT_EQ(object.attributes->relative_velocity.x, object.tracker_data.vcs_vel_rel.x);
   EXPECT_FLOAT_EQ(object.attributes->relative_velocity.y, object.tracker_data.vcs_vel_rel.y);
}


/**
 * Tests functionality intersection point adaption in dependence of object heading. Here small angle saturation shall be used with
 * negative input heading (object approaching from right side). \uts{CSCSA-42173} \sdd{SF-3737} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Adapt_Intersec_Point_To_Object_Heading__small_angles_as_adaption_saturation_obj_approach_from_right)
{
   /** \arrange adaption for rear CTA */
   mode = CTA_MODE_REAR;
   float32_T isect_x_coordinate, isect_adaption;

   p_vehicle_data->host_width  = 2.5f;
   p_vehicle_data->host_length = 6.0f;
   isect_x_coordinate          = -5.0f;

   object.attributes->CTA_heading                                     = -0.9f * p_cals->k_cta_min_park_angle;
   object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] = isect_x_coordinate;

   isect_adaption = (Fbk_Abs_F(0.5f * p_vehicle_data->host_width * Fast_Cos(p_cals->k_cta_min_park_angle)
                               / Fast_Sin(p_cals->k_cta_min_park_angle)));

   /** \action executes function to test */
   Cta_Adapt_Intersec_Point_To_Object_Heading(object.attributes, p_vehicle_data, p_cals, mode);

   /** \assert object shall qualified for criticality level */
   EXPECT_EQ(object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode], isect_x_coordinate - isect_adaption);
}


/**
 * Tests functionality intersection point adaption in dependence of object heading. Here small angle saturation shall be used with
 * positive input heading (object approaching from left). \uts{CSCSA-42174} \sdd{SF-3737} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Adapt_Intersec_Point_To_Object_Heading__small_angles_as_adaption_saturation_obj_approach_from_left)
{
   /** \arrange adaption for rear CTA */
   mode = CTA_MODE_REAR;
   float32_T isect_x_coordinate, isect_adaption;

   p_vehicle_data->host_width  = 2.5f;
   p_vehicle_data->host_length = 6.0f;
   isect_x_coordinate          = -5.0f;

   object.attributes->CTA_heading                                     = 0.9f * p_cals->k_cta_min_park_angle;
   object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] = isect_x_coordinate;

   isect_adaption = (Fbk_Abs_F(0.5f * p_vehicle_data->host_width * Fast_Cos(p_cals->k_cta_min_park_angle)
                               / Fast_Sin(p_cals->k_cta_min_park_angle)));

   /** \action executes function to test */
   Cta_Adapt_Intersec_Point_To_Object_Heading(object.attributes, p_vehicle_data, p_cals, mode);

   /** \assert object shall qualified for criticality level */
   EXPECT_EQ(object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode], isect_x_coordinate - isect_adaption);
}


/**
 * Tests functionality intersection point adaption in dependence of object heading. Here nearly no adaption shall be needed since
 * object is moving perpendicular to host (object approaching from right side). \uts{CSCSA-42175} \sdd{SF-3737}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Adapt_Intersec_Point_To_Object_Heading__perpendicular_movement_no_adaption_obj_approach_from_right)
{
   /** \arrange adaption for rear CTA */
   mode = CTA_MODE_REAR;
   float32_T isect_x_coordinate;

   p_vehicle_data->host_width  = 2.5f;
   p_vehicle_data->host_length = 6.0f;
   isect_x_coordinate          = -5.0f;

   object.attributes->CTA_heading                                     = -0.5f * PI;
   object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] = isect_x_coordinate;

   /** \action executes function to test */
   Cta_Adapt_Intersec_Point_To_Object_Heading(object.attributes, p_vehicle_data, p_cals, mode);

   /** \assert Since object is moving perpendicular the initialized value shall remain */
   EXPECT_EQ(object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode], isect_x_coordinate);
}


/**
 * Tests functionality intersection point adaption in dependence of object heading. Here nearly no adaption shall be needed since
 * object is moving perpendicular to host (object approaching from left side). \uts{CSCSA-42176} \sdd{SF-3737}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Adapt_Intersec_Point_To_Object_Heading__perpendicular_movement_no_adaption_obj_approach_from_left)
{
   /** \arrange adaption for rear CTA */
   mode = CTA_MODE_REAR;
   float32_T isect_x_coordinate;

   p_vehicle_data->host_width  = 2.5f;
   p_vehicle_data->host_length = 6.0f;
   isect_x_coordinate          = -5.0f;

   object.attributes->CTA_heading                                     = 0.5f * PI;
   object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] = isect_x_coordinate;

   /** \action executes function to test */
   Cta_Adapt_Intersec_Point_To_Object_Heading(object.attributes, p_vehicle_data, p_cals, mode);

   /** \assert Since object is moving perpendicular the initialized value shall remain */
   EXPECT_EQ(object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode], isect_x_coordinate);
}


/**
 * Tests functionality intersection point adaption in dependence of object heading. Here the adaption is tested for front cta
 * (object approaching from left side). \uts{CSCSA-42177} \sdd{SF-3737} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Adapt_Intersec_Point_To_Object_Heading__perpendicular_movement_no_adaption_obj_approach_from_left_fcta)
{
   /** \arrange adaption for front CTA */
   mode = CTA_MODE_FRONT;
   float32_T isect_x_coordinate;

   p_vehicle_data->host_width  = 2.5f;
   p_vehicle_data->host_length = 6.0f;
   isect_x_coordinate          = 5.0f;

   object.attributes->CTA_heading                                     = 0.5f * PI;
   object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode] = isect_x_coordinate;

   /** \action executes function to test */
   Cta_Adapt_Intersec_Point_To_Object_Heading(object.attributes, p_vehicle_data, p_cals, mode);

   /** \assert Since object is moving perpendicular the initialized value shall remain */
   EXPECT_EQ(object.attributes->long_isect_point_candidate[FBK_SIDE_LEFT][mode], isect_x_coordinate);
}


/**
 * Checks single invalid object.
 * \uts{CSCSA-42178} \sdd{SF-3702} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Check_Single_Object__check_single_invalid_object)
{
   /** \arrange Invalid object */
   Cta_Inters_Zone_Ext_Param_T confl_zone_ext_params{};

   /** \action executes function to test */
   Cta_Check_Single_Object(&cta_instance, &confl_zone_ext_params, &cta_comparison_data, &object, p_vehicle_data);

   /** \assert check that the object persistent data is reset */
   EXPECT_EQ(object.persistent->prev_cycle_crit_level[mode], (uint8_t) CTA_CRIT_LEVEL_NONE);
   EXPECT_EQ(object.persistent->crit_level_suppression_counter[mode][0], 0);
}

/**
 * Checks single valid object but alert should be suppressed.
 * \uts{CSCSA-42179} \sdd{SF-3702} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Check_Single_Object__check_single_valid_object_suppress_alert)
{
   /** \arrange Valid object */
   Cta_Inters_Zone_Ext_Param_T confl_zone_ext_params{};
   p_cals->k_cta_f_use_heading_for_relative_velocity_calculation = FBK_FALSE;
   p_cals->k_cta_f_use_object_min_object_age_in_cycles           = FBK_FALSE;
   p_cals->k_cta_f_use_object_supress_counter                    = FBK_FALSE;
   p_cals->k_cta_f_check_obstruction_probability_signal          = FBK_FALSE;
   p_cals->k_cta_f_use_ghost_detector                            = FBK_FALSE;
   p_cals->k_cta_f_calc_ttc_ego_side_enabled                     = FBK_FALSE;
   p_cals->k_cta_f_calc_ttp_ego_side_enabled                     = FBK_FALSE;

   object.tracker_data.status                = PA_OBJ_STATUS_MATURE;
   object.tracker_data.vcs_vel_rel.x         = 0.0f;
   object.tracker_data.vcs_vel_rel.y         = Fbk_Half(p_cals->k_cta_max_speed - p_cals->k_cta_min_speed);
   object.tracker_data.speed                 = Fbk_Half(p_cals->k_cta_max_speed - p_cals->k_cta_min_speed);
   object.tracker_data.existence_probability = FBK_ONE_F;

   object.attributes->CTA_heading         = Fbk_Deg_To_Rad(90.0f);
   object.attributes->relative_velocity.y = p_cals->k_cta_min_lateral_approach_speed;

   core_input.cta_stop_mode     = CTA_STOP_MODE_TTP;
   p_cals->k_cta_stop_alert_ttp = 10.0f;

   /** \action executes function to test */
   Cta_Check_Single_Object(&cta_instance, &confl_zone_ext_params, &cta_comparison_data, &object, p_vehicle_data);

   /** \assert check that the object alert should be suppressed */
   EXPECT_TRUE(object.persistent->f_prev_cta_alert_suppress);
}


/**
 * Checks single valid object with TTC in valid range.
 * \uts{CSCSA-42180} \sdd{SF-3702} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Check_Single_Object__check_single_valid_object_valid_ttc)
{
   /** \arrange Valid object */
   Cta_Inters_Zone_Ext_Param_T confl_zone_ext_params{};
   mode                                       = CTA_MODE_REAR;
   confl_zone_ext_params.host_steer_fac[mode] = FBK_ONE_F;
   confl_zone_ext_params.target_head_fac      = FBK_ONE_F;

   p_cals->k_cta_f_use_heading_for_relative_velocity_calculation = FBK_FALSE;
   p_cals->k_cta_f_use_object_min_object_age_in_cycles           = FBK_FALSE;
   p_cals->k_cta_f_use_object_supress_counter                    = FBK_FALSE;
   p_cals->k_cta_f_check_obstruction_probability_signal          = FBK_FALSE;
   p_cals->k_cta_f_use_ghost_detector                            = FBK_FALSE;
   p_cals->k_cta_f_calc_ttc_ego_side_enabled                     = FBK_TRUE;
   p_cals->k_cta_cycle_count_suppress_true_warning               = FBK_ZERO_UINT;
   p_cals->k_cta_f_adapt_intersect_lines_by_host_speed           = FBK_FALSE;


   p_cals->k_cta_min_long_point_criticality_level[mode][0] = 5.0f;
   p_cals->k_cta_max_long_point_criticality_level[mode][0] = -2.5f;
   p_cals->k_cta_ttc_criticality_level[mode][0]            = 1.0f;
   p_cals->k_cta_enable_modes[mode]                        = FBK_TRUE;
   p_cals->k_cta_enable_modes[CTA_MODE_FRONT]              = FBK_FALSE;

   object.tracker_data.status                = PA_OBJ_STATUS_MATURE;
   object.tracker_data.vcs_vel_rel.x         = 0.0f;
   object.tracker_data.vcs_vel_rel.y         = 7.0f;
   object.tracker_data.speed                 = 7.0f;
   object.tracker_data.existence_probability = FBK_ONE_F;
   object.tracker_data.vcs_pos.x             = -8.0f;
   object.tracker_data.vcs_pos.y             = -10.0f;
   object.tracker_data.length                = 5.0f;
   object.tracker_data.width                 = 2.0f;
   object.tracker_data.age                   = 100u;
   object.tracker_data.stage_age             = 100u;

   object.attributes->CTA_heading         = Fbk_Deg_To_Rad(90.0f);
   object.attributes->relative_velocity.y = p_cals->k_cta_min_lateral_approach_speed;

   Cta_Init_Zone(&core_input.cta_zone, p_cals);

   /** \action executes function to test */
   Cta_Check_Single_Object(&cta_instance, &confl_zone_ext_params, &cta_comparison_data, &object, p_vehicle_data);

   /** \assert check that an object alert is set */
   EXPECT_NE(object.persistent->prev_cycle_crit_level[mode], FBK_ZERO_UINT);
}

/**
 * Checks single valid object outside the zone.
 * \uts{CSCSA-188384} \sdd{SF-3702} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Check_Single_Object__check_single_invalid_object_outside_zone)
{
   /** \arrange Valid object outside zone */
   Cta_Inters_Zone_Ext_Param_T confl_zone_ext_params{};
   mode                                       = CTA_MODE_REAR;
   confl_zone_ext_params.host_steer_fac[mode] = FBK_ONE_F;
   confl_zone_ext_params.target_head_fac      = FBK_ONE_F;

   p_cals->k_cta_f_use_heading_for_relative_velocity_calculation = FBK_FALSE;
   p_cals->k_cta_f_use_object_min_object_age_in_cycles           = FBK_FALSE;
   p_cals->k_cta_f_use_object_supress_counter                    = FBK_FALSE;
   p_cals->k_cta_f_check_obstruction_probability_signal          = FBK_FALSE;
   p_cals->k_cta_f_use_ghost_detector                            = FBK_FALSE;
   p_cals->k_cta_f_calc_ttc_ego_side_enabled                     = FBK_TRUE;
   p_cals->k_cta_cycle_count_suppress_true_warning               = FBK_ZERO_UINT;
   p_cals->k_cta_min_long_point_criticality_level[mode][0]       = 5.0f;
   p_cals->k_cta_max_long_point_criticality_level[mode][0]       = -2.5f;
   p_cals->k_cta_ttc_criticality_level[mode][0]                  = 1.0f;
   p_cals->k_cta_enable_modes[mode]                              = FBK_TRUE;
   p_cals->k_cta_enable_modes[CTA_MODE_FRONT]                    = FBK_FALSE;

   object.tracker_data.status                = PA_OBJ_STATUS_MATURE;
   object.tracker_data.id                    = 23u;
   object.tracker_data.vcs_vel_rel.x         = 0.0f;
   object.tracker_data.vcs_vel_rel.y         = 7.0f;
   object.tracker_data.speed                 = 7.0f;
   object.tracker_data.existence_probability = FBK_ONE_F;
   object.tracker_data.vcs_pos.x             = -45.0f;
   object.tracker_data.vcs_pos.y             = -10.0f;
   object.tracker_data.length                = 5.0f;
   object.tracker_data.width                 = 2.0f;
   object.tracker_data.age                   = 100u;
   object.tracker_data.stage_age             = 100u;

   object.attributes->CTA_heading         = Fbk_Deg_To_Rad(90.0f);
   object.attributes->relative_velocity.y = p_cals->k_cta_min_lateral_approach_speed;

   for (uint8_t i = 0; i < p_cals->k_cta_amount_butterfly_points_in_use; i++)
   {
      p_cals->k_cta_butterfly_lat[i] = 0.0;
   }
   Cta_Init_Zone(&core_input.cta_zone, p_cals);

   /** \action executes function to test */
   Cta_Check_Single_Object(&cta_instance, &confl_zone_ext_params, &cta_comparison_data, &object, p_vehicle_data);

   /** \assert check that an object alert is not set */
   EXPECT_EQ(cta_comparison_data.max_level[CTA_MODE_REAR][FBK_SIDE_LEFT], CTA_CRIT_LEVEL_NONE);
   EXPECT_EQ(cta_comparison_data.object_with_highest_crit[CTA_MODE_REAR][FBK_SIDE_LEFT].tracker_data.id, 0);
}


/**
 * Checks single valid object with TTC and speed in valid range.
 * \uts{CSCSA-122777} \sdd{SF-3702} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Check_Single_Object__check_single_valid_object_valid_ttc_speed_above_ths)
{
   /** \arrange Valid object */
   Cta_Inters_Zone_Ext_Param_T confl_zone_ext_params{};
   mode                                       = CTA_MODE_REAR;
   confl_zone_ext_params.host_steer_fac[mode] = FBK_ONE_F;
   confl_zone_ext_params.target_head_fac      = FBK_ONE_F;

   p_cals->k_cta_f_use_heading_for_relative_velocity_calculation = FBK_FALSE;
   p_cals->k_cta_f_use_object_min_object_age_in_cycles           = FBK_FALSE;
   p_cals->k_cta_f_use_object_supress_counter                    = FBK_FALSE;
   p_cals->k_cta_f_check_obstruction_probability_signal          = FBK_FALSE;
   p_cals->k_cta_f_use_ghost_detector                            = FBK_FALSE;
   p_cals->k_cta_f_calc_ttc_ego_side_enabled                     = FBK_TRUE;
   p_cals->k_cta_cycle_count_suppress_true_warning               = FBK_ZERO_UINT;
   p_cals->k_cta_f_adapt_intersect_lines_by_host_speed           = FBK_FALSE;

   p_cals->k_cta_min_long_point_criticality_level[mode][0] = 5.0f;
   p_cals->k_cta_max_long_point_criticality_level[mode][0] = -2.5f;
   p_cals->k_cta_ttc_criticality_level[mode][0]            = 1.0f;
   p_cals->k_cta_enable_modes[mode]                        = FBK_TRUE;
   p_cals->k_cta_enable_modes[CTA_MODE_FRONT]              = FBK_FALSE;
   p_cals->k_cta_speed_criticality_level[mode][0]          = 10.0f;
   p_cals->k_cta_max_speed                                 = 20.0f;

   object.tracker_data.status                = PA_OBJ_STATUS_MATURE;
   object.tracker_data.vcs_vel_rel.x         = 0.0f;
   object.tracker_data.vcs_vel_rel.y         = 15.0f;
   object.tracker_data.speed                 = 15.0f;
   object.tracker_data.existence_probability = FBK_ONE_F;
   object.tracker_data.vcs_pos.x             = -8.0f;
   object.tracker_data.vcs_pos.y             = -10.0f;
   object.tracker_data.length                = 5.0f;
   object.tracker_data.width                 = 2.0f;
   object.tracker_data.age                   = 100u;
   object.tracker_data.stage_age             = 100u;

   object.attributes->CTA_heading         = Fbk_Deg_To_Rad(90.0f);
   object.attributes->relative_velocity.y = p_cals->k_cta_min_lateral_approach_speed;

   core_input.cta_stop_mode = CTA_STOP_MODE_NUM; // wrong value for branch coverage

   Cta_Init_Zone(&core_input.cta_zone, p_cals);

   /** \action executes function to test */
   Cta_Check_Single_Object(&cta_instance, &confl_zone_ext_params, &cta_comparison_data, &object, p_vehicle_data);

   /** \assert check that an object alert is set */
   EXPECT_NE(object.persistent->prev_cycle_crit_level[mode], FBK_ZERO_UINT);
}

/**
 * Checks single valid object with TTC in valid range but speed is below ths.
 * \uts{CSCSA-122778} \sdd{SF-3702} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Check_Single_Object__check_single_valid_object_valid_ttc_speed_below_ths)
{
   /** \arrange Valid object */
   Cta_Inters_Zone_Ext_Param_T confl_zone_ext_params{};
   mode                                       = CTA_MODE_REAR;
   confl_zone_ext_params.host_steer_fac[mode] = FBK_ONE_F;
   confl_zone_ext_params.target_head_fac      = FBK_ONE_F;

   p_cals->k_cta_f_use_heading_for_relative_velocity_calculation = FBK_FALSE;
   p_cals->k_cta_f_use_object_min_object_age_in_cycles           = FBK_FALSE;
   p_cals->k_cta_f_use_object_supress_counter                    = FBK_FALSE;
   p_cals->k_cta_f_check_obstruction_probability_signal          = FBK_FALSE;
   p_cals->k_cta_f_use_ghost_detector                            = FBK_FALSE;
   p_cals->k_cta_f_calc_ttc_ego_side_enabled                     = FBK_TRUE;
   p_cals->k_cta_cycle_count_suppress_true_warning               = FBK_ZERO_UINT;
   p_cals->k_cta_min_long_point_criticality_level[mode][0]       = 5.0f;
   p_cals->k_cta_max_long_point_criticality_level[mode][0]       = -2.5f;
   p_cals->k_cta_ttc_criticality_level[mode][0]                  = 1.0f;
   p_cals->k_cta_enable_modes[mode]                              = FBK_TRUE;
   p_cals->k_cta_enable_modes[CTA_MODE_FRONT]                    = FBK_FALSE;

   p_cals->k_cta_speed_criticality_level[mode][0] = 20.0f;
   p_cals->k_cta_speed_criticality_level[mode][1] = 20.0f;

   object.tracker_data.status                = PA_OBJ_STATUS_MATURE;
   object.tracker_data.vcs_vel_rel.x         = 0.0f;
   object.tracker_data.vcs_vel_rel.y         = 15.0f;
   object.tracker_data.speed                 = 15.0f;
   object.tracker_data.existence_probability = FBK_ONE_F;
   object.tracker_data.vcs_pos.x             = -8.0f;
   object.tracker_data.vcs_pos.y             = -10.0f;
   object.tracker_data.length                = 5.0f;
   object.tracker_data.width                 = 2.0f;
   object.tracker_data.age                   = 100u;
   object.tracker_data.stage_age             = 100u;

   object.attributes->CTA_heading         = Fbk_Deg_To_Rad(90.0f);
   object.attributes->relative_velocity.y = p_cals->k_cta_min_lateral_approach_speed;

   Cta_Init_Zone(&core_input.cta_zone, p_cals);

   /** \action executes function to test */
   Cta_Check_Single_Object(&cta_instance, &confl_zone_ext_params, &cta_comparison_data, &object, p_vehicle_data);

   /** \assert check that an object alert is set */
   EXPECT_EQ(object.persistent->prev_cycle_crit_level[mode], FBK_ZERO_UINT);
}

/**
 * Checks single valid object and set an alert suppress flag.
 * \uts{CSCSA-42181} \sdd{SF-3702} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Check_Single_Object__check_alert_suppress)
{
   /** \arrange Valid object */
   Cta_Inters_Zone_Ext_Param_T confl_zone_ext_params{};
   mode                                       = CTA_MODE_REAR;
   confl_zone_ext_params.host_steer_fac[mode] = FBK_ONE_F;
   confl_zone_ext_params.target_head_fac      = FBK_ONE_F;

   p_cals->k_cta_f_use_heading_for_relative_velocity_calculation = FBK_FALSE;
   p_cals->k_cta_f_use_object_min_object_age_in_cycles           = FBK_FALSE;
   p_cals->k_cta_f_use_object_supress_counter                    = FBK_FALSE;
   p_cals->k_cta_f_check_obstruction_probability_signal          = FBK_FALSE;
   p_cals->k_cta_f_use_ghost_detector                            = FBK_FALSE;
   p_cals->k_cta_f_calc_ttc_ego_side_enabled                     = FBK_TRUE;
   p_cals->k_cta_cycle_count_suppress_true_warning               = FBK_ZERO_UINT;
   p_cals->k_cta_min_long_point_criticality_level[mode][0]       = 5.0f;
   p_cals->k_cta_max_long_point_criticality_level[mode][0]       = -2.5f;
   p_cals->k_cta_ttc_criticality_level[mode][0]                  = 1.0f;
   p_cals->k_cta_enable_modes[mode]                              = FBK_TRUE;
   p_cals->k_cta_enable_modes[CTA_MODE_FRONT]                    = FBK_FALSE;

   p_cals->k_cta_dist_thres_crit_level_reset     = 10.0f;
   p_cals->k_cta_f_enable_thres_crit_level_reset = FBK_TRUE;
   p_cals->k_cta_f_use_front_corners_dist_stop   = FBK_TRUE;
   object.tracker_data.status                    = PA_OBJ_STATUS_MATURE;
   object.tracker_data.vcs_vel_rel.x             = 0.0f;
   object.tracker_data.vcs_vel_rel.y             = 7.0f;
   object.tracker_data.speed                     = 7.0f;
   object.tracker_data.existence_probability     = FBK_ONE_F;
   object.tracker_data.vcs_pos.x                 = -8.0f;
   object.tracker_data.vcs_pos.y                 = -10.0f;
   object.tracker_data.length                    = 5.0f;
   object.tracker_data.width                     = 2.0f;
   object.tracker_data.age                       = 100u;
   object.tracker_data.stage_age                 = 100u;

   object.attributes->CTA_heading         = Fbk_Deg_To_Rad(90.0f);
   object.attributes->relative_velocity.y = p_cals->k_cta_min_lateral_approach_speed;

   Cta_Init_Zone(&core_input.cta_zone, p_cals);

   /** \action executes function to test */
   Cta_Check_Single_Object(&cta_instance, &confl_zone_ext_params, &cta_comparison_data, &object, p_vehicle_data);

   /** \assert check that flag to suppress alert is set to True */
   EXPECT_TRUE(object.persistent->f_prev_cta_alert_suppress);
}

/**
 * Checks that the core output is set correctly.
 * \uts{CSCSA-42182} \sdd{SF-3703} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Set_Core_Output__set_core_output_correctly)
{

   /** \arrange Set up cals and pointers */
   Cta_Status_T cta_status  = CTA_STATUS_ACTIVE;
   p_cals->k_cta_enable_ctb = FBK_TRUE;

   obj_tracker_output.id                                                           = 3u;
   obj_tracker_output.unique_id                                                    = 3u;
   cta_comparison_data.object_with_highest_crit[mode][FBK_SIDE_LEFT].attributes    = &attributes;
   cta_comparison_data.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes   = &attributes;
   cta_comparison_data.object_with_highest_crit[mode][FBK_SIDE_LEFT].tracker_data  = obj_tracker_output;
   obj_tracker_output.id                                                           = 2u;
   obj_tracker_output.unique_id                                                    = 2u;
   cta_comparison_data.object_with_highest_crit[mode][FBK_SIDE_RIGHT].tracker_data = obj_tracker_output;

   /** \action executes function to test */
   Cta_Set_Core_Output(&cta_instance, &cta_comparison_data, p_vehicle_data, cta_status);

   /** \assert check core output */
   EXPECT_EQ(core_output.cta_status, cta_status);
   EXPECT_EQ(core_output.cta_id[mode][FBK_SIDE_LEFT], 3u);
   EXPECT_EQ(core_output.cta_id[mode][FBK_SIDE_RIGHT], 2u);
   EXPECT_EQ(core_output.cta_unique_id[mode][FBK_SIDE_LEFT], 3u);
   EXPECT_EQ(core_output.cta_unique_id[mode][FBK_SIDE_RIGHT], 2u);
}

/**
 * Checks that the core output is not set for inavlid ID.
 * \uts{CSCSA-188385} \sdd{SF-3703} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Set_Core_Output__set_core_output_invalid_id)
{

   /** \arrange Set up cals and pointers */
   Cta_Status_T cta_status  = CTA_STATUS_ACTIVE;
   p_cals->k_cta_enable_ctb = FBK_FALSE;

   obj_tracker_output.id                                                           = 0u;
   obj_tracker_output.unique_id                                                    = 0u;
   cta_comparison_data.object_with_highest_crit[mode][FBK_SIDE_LEFT].attributes    = &attributes;
   cta_comparison_data.object_with_highest_crit[mode][FBK_SIDE_RIGHT].attributes   = &attributes;
   cta_comparison_data.object_with_highest_crit[mode][FBK_SIDE_LEFT].tracker_data  = obj_tracker_output;
   obj_tracker_output.id                                                           = 0u;
   obj_tracker_output.unique_id                                                    = 0u;
   cta_comparison_data.object_with_highest_crit[mode][FBK_SIDE_RIGHT].tracker_data = obj_tracker_output;

   /** \action executes function to test */
   Cta_Set_Core_Output(&cta_instance, &cta_comparison_data, p_vehicle_data, cta_status);

   /** \assert check core output */
   EXPECT_EQ(core_output.cta_status, cta_status);
   EXPECT_EQ(core_output.cta_id[mode][FBK_SIDE_LEFT], 0u);
   EXPECT_EQ(core_output.cta_id[mode][FBK_SIDE_RIGHT], 0u);
}

/**
 * Checks if selected parsistent data are mapped to core output correctly.
 * \uts{CSCSA-42183} \sdd{SF-3703} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Set_Core_Output__map_persitent_data_to_core_output)
{

   /** \arrange Set up cals and pointers */
   Cta_Status_T cta_status  = CTA_STATUS_ACTIVE;
   p_cals->k_cta_enable_ctb = FBK_TRUE;

   cta_persistent.warning_holding_counter[CTA_MODE_REAR][FBK_SIDE_LEFT]   = 10u;
   cta_persistent.warning_holding_counter[CTA_MODE_REAR][FBK_SIDE_RIGHT]  = 11u;
   cta_persistent.warning_holding_counter[CTA_MODE_FRONT][FBK_SIDE_LEFT]  = 12u;
   cta_persistent.warning_holding_counter[CTA_MODE_FRONT][FBK_SIDE_RIGHT] = 13u;

   cta_persistent.brake_holding_counter[CTA_MODE_REAR][FBK_SIDE_LEFT]   = 14u;
   cta_persistent.brake_holding_counter[CTA_MODE_REAR][FBK_SIDE_RIGHT]  = 15u;
   cta_persistent.brake_holding_counter[CTA_MODE_FRONT][FBK_SIDE_LEFT]  = 16u;
   cta_persistent.brake_holding_counter[CTA_MODE_FRONT][FBK_SIDE_RIGHT] = 17u;

   cta_persistent.brake_suppression_counter[CTA_MODE_REAR][FBK_SIDE_LEFT]   = 18u;
   cta_persistent.brake_suppression_counter[CTA_MODE_REAR][FBK_SIDE_RIGHT]  = 19u;
   cta_persistent.brake_suppression_counter[CTA_MODE_FRONT][FBK_SIDE_LEFT]  = 20u;
   cta_persistent.brake_suppression_counter[CTA_MODE_FRONT][FBK_SIDE_RIGHT] = 21u;


   /** \action executes function to test */
   Cta_Set_Core_Output(&cta_instance, &cta_comparison_data, p_vehicle_data, cta_status);

   /** \assert check core output */
   EXPECT_EQ(core_output.cta_status, cta_status);
   EXPECT_EQ(core_output.cta_warn_hold_cnt[CTA_MODE_REAR][FBK_SIDE_LEFT], 0u);
   EXPECT_EQ(core_output.cta_warn_hold_cnt[CTA_MODE_REAR][FBK_SIDE_RIGHT], 0u);
   EXPECT_EQ(core_output.cta_brake_hold_cnt[CTA_MODE_REAR][FBK_SIDE_LEFT], 14u);
   EXPECT_EQ(core_output.cta_brake_hold_cnt[CTA_MODE_REAR][FBK_SIDE_RIGHT], 15u);
   EXPECT_EQ(core_output.cta_brake_supp_cnt[CTA_MODE_REAR][FBK_SIDE_LEFT], 0u);
   EXPECT_EQ(core_output.cta_brake_supp_cnt[CTA_MODE_REAR][FBK_SIDE_RIGHT], 0u);
}


/**
 * Checks that ref point is correctly calculated. Here, the predicted intersection point is out of range
 * \uts{CSCSA-42187} \sdd{CSCSA-16663} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Reference_Point_And_Fill_Properties__pred_out_zone)
{
   /** \arrange Set up cals and pointers */
   target_corners.points[0]                  = Create_2d_Vector_Coordinates(-3.0f, 10.0f);
   target_corners.points[1]                  = Create_2d_Vector_Coordinates(-2.0f, 10.0f);
   target_corners.points[2]                  = Create_2d_Vector_Coordinates(-1.0f, 10.0f);
   target_corners.points[3]                  = Create_2d_Vector_Coordinates(-1.0f, 12.0f);
   target_corners.points[4]                  = Create_2d_Vector_Coordinates(-1.0f, 14.0f);
   target_corners.points[5]                  = Create_2d_Vector_Coordinates(-2.0f, 14.0f);
   target_corners.points[6]                  = Create_2d_Vector_Coordinates(-3.0f, 14.0f);
   target_corners.points[7]                  = Create_2d_Vector_Coordinates(-3.0f, 12.0f);
   object.tracker_data.vcs_vel_rel.y         = -2.0f;
   object.attributes->relative_velocity.y    = -2.0f;
   object.tracker_data.vcs_vel_rel.x         = 0.0f;
   p_cals->k_cta_f_calc_ttc_ego_side_enabled = FBK_FALSE;
   p_cals->k_cta_f_calc_ttp_ego_side_enabled = FBK_FALSE;
   /** \action executes function to test */
   Cta_Get_Reference_Point_And_Fill_Properties(&object, p_vehicle_data, &target_corners, p_cals);

   /** \assert check ref point */
   EXPECT_EQ(object.attributes->ref_point_candidate[FBK_SIDE_LEFT].ref_point_index, FBK_FRONT_LEFT_CORNER);
   EXPECT_EQ(object.attributes->ref_point_ttp.ref_point_index, FBK_REAR_RIGHT_CORNER);
}

/**
 * Checks that ref point is correctly calculated. Here predicted intersection point is in range;
 * \uts{CSCSA-85515} \sdd{CSCSA-16663} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Reference_Point_And_Fill_Properties__pred_in_range)
{
   /** \arrange Set up cals and pointers */
   target_corners.points[0]                  = Create_2d_Vector_Coordinates(-3.0f, 10.0f);
   target_corners.points[1]                  = Create_2d_Vector_Coordinates(-2.0f, 10.0f);
   target_corners.points[2]                  = Create_2d_Vector_Coordinates(-1.0f, 10.0f);
   target_corners.points[3]                  = Create_2d_Vector_Coordinates(-1.0f, 12.0f);
   target_corners.points[4]                  = Create_2d_Vector_Coordinates(-1.0f, 14.0f);
   target_corners.points[5]                  = Create_2d_Vector_Coordinates(-2.0f, 14.0f);
   target_corners.points[6]                  = Create_2d_Vector_Coordinates(-3.0f, 14.0f);
   target_corners.points[7]                  = Create_2d_Vector_Coordinates(-3.0f, 12.0f);
   object.tracker_data.vcs_vel_rel.y         = -2.0f;
   object.attributes->relative_velocity.y    = -2.0f;
   object.tracker_data.vcs_vel_rel.x         = 0.0f;
   p_cals->k_cta_f_calc_ttc_ego_side_enabled = FBK_FALSE;
   p_cals->k_cta_f_calc_ttp_ego_side_enabled = FBK_FALSE;


   /** \action executes function to test */
   Cta_Get_Reference_Point_And_Fill_Properties(&object, p_vehicle_data, &target_corners, p_cals);

   /** \assert check ref point */
   EXPECT_EQ(object.attributes->ref_point_candidate[FBK_SIDE_LEFT].ref_point_index, FBK_FRONT_LEFT_CORNER);
   EXPECT_EQ(object.attributes->ref_point_ttp.ref_point_index, FBK_REAR_RIGHT_CORNER);
}
/**
 * Checks that ref point is correctly calculated. Here, side shift is used for ttp and ttc calculation
 * \uts{CSCSA-42188} \sdd{CSCSA-16663} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Reference_Point_And_Fill_Properties__use_side_shift)
{
   /** \arrange Set up cals and pointers */
   target_corners.points[0]                  = Create_2d_Vector_Coordinates(-3.0f, 10.0f);
   target_corners.points[1]                  = Create_2d_Vector_Coordinates(-2.0f, 10.0f);
   target_corners.points[2]                  = Create_2d_Vector_Coordinates(-1.0f, 10.0f);
   target_corners.points[3]                  = Create_2d_Vector_Coordinates(-1.0f, 12.0f);
   target_corners.points[4]                  = Create_2d_Vector_Coordinates(-1.0f, 14.0f);
   target_corners.points[5]                  = Create_2d_Vector_Coordinates(-2.0f, 14.0f);
   target_corners.points[6]                  = Create_2d_Vector_Coordinates(-3.0f, 14.0f);
   target_corners.points[7]                  = Create_2d_Vector_Coordinates(-3.0f, 12.0f);
   object.tracker_data.vcs_vel_rel.y         = -2.0f;
   object.attributes->relative_velocity.y    = -2.0f;
   object.tracker_data.vcs_vel_rel.x         = 0.0f;
   p_cals->k_cta_f_calc_ttc_ego_side_enabled = FBK_TRUE;
   p_cals->k_cta_f_calc_ttp_ego_side_enabled = FBK_TRUE;

   /** \action executes function to test */
   Cta_Get_Reference_Point_And_Fill_Properties(&object, p_vehicle_data, &target_corners, p_cals);

   /** \assert check ref point */
   EXPECT_EQ(object.attributes->ref_point_candidate[FBK_SIDE_LEFT].ref_point_index, FBK_FRONT_LEFT_CORNER);
   EXPECT_EQ(object.attributes->ref_point_ttp.ref_point_index, FBK_REAR_RIGHT_CORNER);
}

/**
 * Checks that ref point is correctly calculated. Here, object speed is too low, below zero
 * \uts{CSCSA-85516} \sdd{CSCSA-16663} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Reference_Point_And_Fill_Properties__speed_to_low_negative)
{
   /** \arrange Set up cals and pointers */
   target_corners.points[0]                  = Create_2d_Vector_Coordinates(-3.0f, 10.0f);
   target_corners.points[1]                  = Create_2d_Vector_Coordinates(-2.0f, 10.0f);
   target_corners.points[2]                  = Create_2d_Vector_Coordinates(-1.0f, 10.0f);
   target_corners.points[3]                  = Create_2d_Vector_Coordinates(-1.0f, 12.0f);
   target_corners.points[4]                  = Create_2d_Vector_Coordinates(-1.0f, 14.0f);
   target_corners.points[5]                  = Create_2d_Vector_Coordinates(-2.0f, 14.0f);
   target_corners.points[6]                  = Create_2d_Vector_Coordinates(-3.0f, 14.0f);
   target_corners.points[7]                  = Create_2d_Vector_Coordinates(-3.0f, 12.0f);
   object.tracker_data.vcs_vel_rel.y         = -Fbk_Half(EPSILON);
   object.attributes->relative_velocity.y    = -Fbk_Half(EPSILON);
   object.tracker_data.vcs_vel_rel.x         = 0.0f;
   p_cals->k_cta_f_calc_ttc_ego_side_enabled = FBK_FALSE;
   p_cals->k_cta_f_calc_ttp_ego_side_enabled = FBK_FALSE;

   /** \action executes function to test */
   Cta_Get_Reference_Point_And_Fill_Properties(&object, p_vehicle_data, &target_corners, p_cals);

   /** \assert check ref point */
   EXPECT_EQ(object.attributes->ref_point_candidate[FBK_SIDE_LEFT].ref_point_index, FBK_FRONT_LEFT_CORNER);
   EXPECT_EQ(object.attributes->ref_point_ttp.ref_point_index, FBK_REAR_MID);
}

/**
 * Checks that ref point is correctly calculated. Here, object speed is too low, above zero
 * \uts{CSCSA-85517} \sdd{CSCSA-16663} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Get_Reference_Point_And_Fill_Properties__speed_to_low_positive)
{
   /** \arrange Set up cals and pointers */
   target_corners.points[0]                  = Create_2d_Vector_Coordinates(-3.0f, 10.0f);
   target_corners.points[1]                  = Create_2d_Vector_Coordinates(-2.0f, 10.0f);
   target_corners.points[2]                  = Create_2d_Vector_Coordinates(-1.0f, 10.0f);
   target_corners.points[3]                  = Create_2d_Vector_Coordinates(-1.0f, 12.0f);
   target_corners.points[4]                  = Create_2d_Vector_Coordinates(-1.0f, 14.0f);
   target_corners.points[5]                  = Create_2d_Vector_Coordinates(-2.0f, 14.0f);
   target_corners.points[6]                  = Create_2d_Vector_Coordinates(-3.0f, 14.0f);
   target_corners.points[7]                  = Create_2d_Vector_Coordinates(-3.0f, 12.0f);
   object.tracker_data.vcs_vel_rel.y         = Fbk_Half(EPSILON);
   object.attributes->relative_velocity.y    = Fbk_Half(EPSILON);
   object.tracker_data.vcs_vel_rel.x         = 0.0f;
   p_cals->k_cta_f_calc_ttc_ego_side_enabled = FBK_FALSE;
   p_cals->k_cta_f_calc_ttp_ego_side_enabled = FBK_FALSE;

   /** \action executes function to test */
   Cta_Get_Reference_Point_And_Fill_Properties(&object, p_vehicle_data, &target_corners, p_cals);

   /** \assert check ref point */
   EXPECT_EQ(object.attributes->ref_point_candidate[FBK_SIDE_LEFT].ref_point_index, FBK_FRONT_LEFT_CORNER);
   EXPECT_EQ(object.attributes->ref_point_ttp.ref_point_index, FBK_REAR_MID);
}

/**
 * Tests the persistent data is initialized for new object, due to age equal one and status new.
 * \uts{CSCSA-188386} \sdd{SF-3701} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Algorithm__reset_single_persistent_data)
{
   /** \arrange object with a sample configuration for TTP calculation */
   data.object_data[0].age                                        = 1;
   p_obj_persistent_array[0].prev_cycle_crit_level[CTA_MODE_REAR] = CTA_CRIT_LEVEL_2;

   data.object_data[1].age                                        = 3;
   data.object_data[1].status                                     = PA_OBJ_STATUS_NEW;
   p_obj_persistent_array[0].prev_cycle_crit_level[CTA_MODE_REAR] = CTA_CRIT_LEVEL_2;

   /** \action executes function to test */
   Cta_Algorithm(&cta_instance, &cta_comparison_data, p_vehicle_data);

   /** \assert expect equality of returned ttp and calculated */
   EXPECT_FLOAT_EQ(p_obj_persistent_array[0].prev_cycle_crit_level[CTA_MODE_REAR], CTA_CRIT_LEVEL_NONE);
   EXPECT_FLOAT_EQ(p_obj_persistent_array[1].prev_cycle_crit_level[CTA_MODE_REAR], CTA_CRIT_LEVEL_NONE);
}

/**
 * Tests the algorithm is exectued on objects with coasted and mature status.
 * \uts{CSCSA-188387} \sdd{SF-3701} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Test, Cta_Algorithm__run_algorithm_on_coasted_and_mature_objects)
{
   /** \arrange Create critial mature and coasted object on right and left side */
   p_cals->k_cta_cycle_count_suppress_true_warning     = 0u;
   p_cals->k_cta_f_use_rel_vel_isect_point_calc        = FBK_TRUE;
   p_cals->k_cta_min_mature_cycles_level_qualifiction  = 0u;
   p_cals->k_cta_f_adapt_intersect_lines_by_host_speed = 0u;

   p_cals->k_cta_min_long_point_criticality_level[CTA_MODE_REAR][CTA_CRIT_LEVEL_1 - 1] = 5.0f;
   p_cals->k_cta_max_long_point_criticality_level[CTA_MODE_REAR][CTA_CRIT_LEVEL_1 - 1] = -2.5f;
   p_cals->k_cta_min_long_point_criticality_level[CTA_MODE_REAR][CTA_CRIT_LEVEL_2 - 1] = 5.0f;
   p_cals->k_cta_max_long_point_criticality_level[CTA_MODE_REAR][CTA_CRIT_LEVEL_2 - 1] = -2.5f;

   data.object_data[0].status                = PA_OBJ_STATUS_COASTED;
   data.object_data[0].id                    = 69u;
   data.object_data[0].f_moveable            = FBK_TRUE;
   data.object_data[0].vcs_vel_rel.x         = 0.0f;
   data.object_data[0].vcs_heading           = 1.35f;
   data.object_data[0].vcs_vel_rel.y         = 7.0f;
   data.object_data[0].speed                 = 7.0f;
   data.object_data[0].existence_probability = FBK_ONE_F;
   data.object_data[0].vcs_pos.x             = -10.0f;
   data.object_data[0].vcs_pos.y             = -10.0f;
   data.object_data[0].length                = 5.0f;
   data.object_data[0].width                 = 2.0f;
   data.object_data[0].age                   = 100u;
   object.tracker_data.stage_age             = 100u;

   data.object_data[1].status                = PA_OBJ_STATUS_MATURE;
   data.object_data[1].id                    = 17u;
   data.object_data[1].f_moveable            = FBK_TRUE;
   data.object_data[1].vcs_vel_rel.x         = 0.0f;
   data.object_data[1].vcs_heading           = -1.35f;
   data.object_data[1].vcs_vel_rel.y         = -7.0f;
   data.object_data[1].speed                 = 7.0f;
   data.object_data[1].existence_probability = FBK_ONE_F;
   data.object_data[1].vcs_pos.x             = -10.0f;
   data.object_data[1].vcs_pos.y             = 10.0f;
   data.object_data[1].length                = 5.0f;
   data.object_data[1].width                 = 2.0f;
   data.object_data[1].age                   = 100u;
   object.tracker_data.stage_age             = 100u;

   /** \action executes function to test */
   Cta_Algorithm(&cta_instance, &cta_comparison_data, p_vehicle_data);

   /** \assert expect equality of cta comparision data */
   EXPECT_NE(cta_comparison_data.max_level[CTA_MODE_REAR][FBK_SIDE_LEFT], CTA_CRIT_LEVEL_NONE);
   EXPECT_EQ(cta_comparison_data.object_with_highest_crit[CTA_MODE_REAR][FBK_SIDE_LEFT].tracker_data.id, data.object_data[0].id);
   EXPECT_NE(cta_comparison_data.max_level[CTA_MODE_REAR][FBK_SIDE_RIGHT], CTA_CRIT_LEVEL_NONE);
   EXPECT_EQ(cta_comparison_data.object_with_highest_crit[CTA_MODE_REAR][FBK_SIDE_RIGHT].tracker_data.id, data.object_data[1].id);
}