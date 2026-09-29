#ifndef TA_OBJECT_FILTER_TEST_HPP
#define TA_OBJECT_FILTER_TEST_HPP

/**
 * @file ta_object_filter_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for TA unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include <gtest/gtest.h> // IWYU pragma: keep
#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "ta_core_calibration.h"
#include "ta_core_input_t.h"
#include "ta_persistent_t.h"
#include "ta_types.h"
}

/**
 * Class used to create a fixture for TA test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ta_Object_Filter_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Ta_Object_T ta_object{};
   Ta_Core_Input_T ta_core_input{};
   Ta_Persistent_T ta_persistent{};
   Ta_Core_Calibration_T ta_cal;
   Pa_Context_T ta_context{};
   Pa_Data_T data{};
   Fbk_Vehicle_Data_T *p_vehicle_data;

   void SetUp() override
   {
      /* Initialize context data */
      ta_context.p_data = &data;
      p_vehicle_data    = &(ta_context.p_data->vehicle_data);

      ta_core_input.p_pa_data    = &data;
      ta_core_input.f_fta_enable = FBK_TRUE;
      ta_core_input.f_rta_enable = FBK_TRUE;

      Ta_Core_Cal_Update_Defaults(&ta_cal);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

   void Ta_Set_Up_Fta_Relevant_Object(Ta_Object_T *p_ta_object, Ta_Core_Calibration_T *p_cals);

   void Ta_Set_Up_Rta_Relevant_Object(Ta_Object_T *p_ta_object, Ta_Core_Calibration_T *p_cals);
};


void inline Ta_Object_Filter_Test::Ta_Set_Up_Fta_Relevant_Object(Ta_Object_T *p_ta_object, Ta_Core_Calibration_T *p_cals)
{
   /* Arrange */

   p_vehicle_data->curvature = -0.1f;

   p_ta_object->attributes.f_obj_in_danger_zone         = FBK_FALSE;
   p_ta_object->tracker_data.existence_probability      = 1.0f;
   p_ta_object->tracker_data.vcs_vel_rel.x              = 1.0f;
   p_ta_object->tracker_data.vcs_vel_rel.y              = 0.0f;
   p_ta_object->tracker_data.vcs_vel.x                  = 10.0f;
   p_ta_object->tracker_data.vcs_vel.y                  = 0.0f;
   p_ta_object->tracker_data.speed                      = 10.0f;
   p_ta_object->tracker_data.length                     = 1.5f;
   p_ta_object->tracker_data.width                      = 1.5f;
   p_ta_object->tracker_data.obj_class                  = PA_OBJ_CLASS_PEDESTRIAN;
   p_ta_object->attributes.object_class_probability_vru = 1.0f;
   p_ta_object->tracker_data.age                        = 10u;

   p_cals->k_f_fta_enable                     = 1;
   p_cals->k_fta_obj_exist_prblty[TA_MIN]     = 0.6f;
   p_cals->k_fta_obj_exist_prblty[TA_MAX]     = 1.0f;
   p_cals->k_fta_obj_vcs_long_vel_rel[TA_MIN] = 0.0f;
   p_cals->k_fta_obj_vcs_long_vel_rel[TA_MAX] = 2.0f;
   p_cals->k_fta_obj_vcs_lat_vel_rel[TA_MIN]  = 0.0f;
   p_cals->k_fta_obj_vcs_lat_vel_rel[TA_MAX]  = 1.0f;
   p_cals->k_fta_obj_vcs_long_vel[TA_MIN]     = 1.0f;
   p_cals->k_fta_obj_vcs_long_vel[TA_MAX]     = 15.0f;
   p_cals->k_fta_obj_vcs_lat_vel[TA_MIN]      = 0.0f;
   p_cals->k_fta_obj_vcs_lat_vel[TA_MAX]      = 1.0f;
   p_cals->k_fta_obj_heading[TA_MIN]          = 0.0f;
   p_cals->k_fta_obj_heading[TA_MAX]          = 1.0f;
   p_cals->k_fta_obj_speed[TA_MIN]            = 1.0f;
   p_cals->k_fta_obj_speed[TA_MAX]            = 20.0f;
   p_cals->k_fta_obj_vru_class_prob[TA_MIN]   = 0.5f;
   p_cals->k_fta_obj_vru_class_prob[TA_MAX]   = 1.0f;
   p_cals->k_fta_obj_length[TA_MAX]           = p_ta_object->tracker_data.length;
   p_cals->k_fta_obj_width[TA_MAX]            = p_ta_object->tracker_data.width;
   p_cals->k_fta_obj_age_min                  = p_ta_object->tracker_data.age;

   /* Set up positive danger zone check */
   p_ta_object->tracker_data.vcs_pos.x      = 2.0f;
   p_ta_object->tracker_data.vcs_pos.y      = -5.0f;
   p_ta_object->tracker_data.vcs_heading    = 0.3f;
   p_ta_object->attributes.velocity_heading = p_ta_object->tracker_data.vcs_heading;

   p_cals->k_f_fta_enable_danger_zones  = 1;
   p_cals->k_fta_danger_zone_point_size = 4;

   p_cals->k_fta_danger_zone_left_long[0] = 8.0f;
   p_cals->k_fta_danger_zone_left_long[1] = 8.0f;
   p_cals->k_fta_danger_zone_left_long[2] = -4.0f;
   p_cals->k_fta_danger_zone_left_long[3] = -4.0f;

   p_cals->k_fta_danger_zone_left_lat[0] = -10.0f;
   p_cals->k_fta_danger_zone_left_lat[1] = 0.0f;
   p_cals->k_fta_danger_zone_left_lat[2] = 0.0f;
   p_cals->k_fta_danger_zone_left_lat[3] = -10.0f;
}

void inline Ta_Object_Filter_Test::Ta_Set_Up_Rta_Relevant_Object(Ta_Object_T *p_ta_object, Ta_Core_Calibration_T *p_cals)
{
   /* Arrange */
   p_ta_object->attributes.f_obj_in_info_zone           = FBK_FALSE;
   p_ta_object->attributes.f_obj_in_wing_zone           = FBK_FALSE;
   p_ta_object->tracker_data.existence_probability      = 1.0f;
   p_ta_object->tracker_data.vcs_vel_rel.x              = 1.0f;
   p_ta_object->tracker_data.vcs_vel_rel.y              = 0.0f;
   p_ta_object->tracker_data.vcs_vel.x                  = 10.0f;
   p_ta_object->tracker_data.vcs_vel.y                  = 0.0f;
   p_ta_object->tracker_data.vcs_heading                = 0.0f;
   p_ta_object->tracker_data.speed                      = 10.0f;
   p_ta_object->tracker_data.length                     = 2.0f;
   p_ta_object->tracker_data.width                      = 2.0f;
   p_ta_object->tracker_data.obj_class                  = PA_OBJ_CLASS_PEDESTRIAN;
   p_ta_object->attributes.object_class_probability_vru = 1.0f;
   p_ta_object->tracker_data.age                        = 10u;
   p_ta_object->tracker_data.f_reflection               = FBK_FALSE;

   p_cals->k_f_rta_enable                     = 1;
   p_cals->k_rta_obj_exist_prblty[TA_MIN]     = 0.6f;
   p_cals->k_rta_obj_exist_prblty[TA_MAX]     = 1.0f;
   p_cals->k_rta_obj_vcs_long_vel_rel[TA_MIN] = 0.0f;
   p_cals->k_rta_obj_vcs_long_vel_rel[TA_MAX] = 2.0f;
   p_cals->k_rta_obj_vcs_lat_vel_rel[TA_MIN]  = 0.0f;
   p_cals->k_rta_obj_vcs_lat_vel_rel[TA_MAX]  = 1.0f;
   p_cals->k_rta_obj_vcs_long_vel[TA_MIN]     = 1.0f;
   p_cals->k_rta_obj_vcs_long_vel[TA_MAX]     = 15.0f;
   p_cals->k_rta_obj_vcs_lat_vel[TA_MIN]      = 0.0f;
   p_cals->k_rta_obj_vcs_lat_vel[TA_MAX]      = 1.0f;
   p_cals->k_rta_obj_heading[TA_MIN]          = 0.0f;
   p_cals->k_rta_obj_heading[TA_MAX]          = 1.0f;
   p_cals->k_rta_obj_speed[TA_MIN]            = 1.0f;
   p_cals->k_rta_obj_speed[TA_MAX]            = 20.0f;
   p_cals->k_rta_obj_vru_class_prob[TA_MIN]   = 0.5f;
   p_cals->k_rta_obj_vru_class_prob[TA_MAX]   = 1.0f;

   /* Set up positive info and wing zone check */
   p_ta_object->attributes.f_curvi_available = FBK_FALSE;
   p_ta_object->tracker_data.vcs_pos.x       = -8.0f;
   p_ta_object->tracker_data.vcs_pos.y       = -5.0f;

   p_cals->k_f_rta_enable_info_zones  = 1;
   p_cals->k_rta_info_zone_point_size = 4;

   p_cals->k_rta_info_zone_left_long[0] = -1.0f;
   p_cals->k_rta_info_zone_left_long[1] = -1.0f;
   p_cals->k_rta_info_zone_left_long[2] = -26.0f;
   p_cals->k_rta_info_zone_left_long[3] = -26.0f;

   p_cals->k_rta_info_zone_left_lat[0] = -7.0f;
   p_cals->k_rta_info_zone_left_lat[1] = -1.0f;
   p_cals->k_rta_info_zone_left_lat[2] = -1.0f;
   p_cals->k_rta_info_zone_left_lat[3] = -7.0f;

   p_cals->k_f_rta_enable_wing_zones  = 1;
   p_cals->k_rta_wing_zone_point_size = 4;

   p_cals->k_rta_wing_zone_left_long[0] = -1.0f;
   p_cals->k_rta_wing_zone_left_long[1] = -1.0f;
   p_cals->k_rta_wing_zone_left_long[2] = -10.0f;
   p_cals->k_rta_wing_zone_left_long[3] = -10.0f;

   p_cals->k_rta_wing_zone_left_lat[0] = -10.0f;
   p_cals->k_rta_wing_zone_left_lat[1] = -1.0f;
   p_cals->k_rta_wing_zone_left_lat[2] = -1.0f;
   p_cals->k_rta_wing_zone_left_lat[3] = -10.0f;

   /* Host vehicle is turning */
   p_vehicle_data->curvature = p_cals->k_ta_straight_host_curvature_max + EPSILON;
}
#endif /*TA_OBJECT_FILTER_TEST_HPP*/
