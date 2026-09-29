#ifndef CED_COMMON_FUNCTIONS_H
#define CED_COMMON_FUNCTIONS_H

/**
 * @file ced_common_functions_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for CED unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest.h> // IWYU pragma: keep

extern "C"
{
#include "ced.h"
#include "ced_common_functions.h" // IWYU pragma: keep
#include "ced_core_calibration.h"
#include "ced_core_input_t.h"
#include "ced_core_output_t.h"
#include "ced_create_zones.h"
#include "ced_input_t.h"
#include "ced_instance.h"
#include "ced_pre_run.h"
#include "ced_types.h"
#include "fbk_field_of_interest.h"
#include "fbk_index_lookup.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "fbk_vehicle_data_t.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "pt_output_t.h"
}

/**
 * Class used to create a fixture for CED test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ced_Common_Functions_Test : public ::testing::Test
{
 protected:
   Pt_Output_T pt_output{};
   Pt_Path_Object_Pair_Output_T path_obj_pair_output{};

   Ced_Instance_T ced_instance;

   Ced_Core_Calibration_T &ced_cals{ced_instance.calibration};
   Ced_Core_Input_T ced_core_input{ced_instance.core_input};
   Ced_Core_Output_T ced_core_output{ced_instance.core_output};
   Ced_Persistent_T ced_persistance{ced_instance.persistance};

   Ced_Input_T ced_input{};

   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;

   Fbk_Output_T fbk_output{};
   Fbk_Index_Id_Lookup_Table_T fbk_lookup_table;
   Fbk_Field_Of_Interest_T ced_funnel_zone{};
   Fbk_Field_Of_Interest_T ced_collision_zone{};
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp()
   {
      /* Init cals */
      Ced_Core_Cal_Update_Defaults(&ced_cals);
      ced_cals.k_ced_funnel_zone_length                                  = 60.0f;
      ced_cals.k_ced_funnel_zone_width                                   = 20.0f;
      ced_cals.k_ced_collision_zone_width                                = 1.5f;
      ced_cals.k_ced_ego_lane_width                                      = 1.0f;
      ced_cals.k_ced_f_path_tracking_enable                              = FBK_TRUE;
      ced_cals.k_ced_f_second_warning_level_enable                       = FBK_TRUE;
      ced_cals.k_ced_object_width_safety_margin_for_active_alert         = 0.0f;
      ced_cals.k_ced_object_width_safety_margin_for_critical_path_match  = 0.0f;
      ced_cals.k_ced_object_max_width_increase_factor_with_path_match    = 0.0f;
      ced_cals.k_ced_object_max_width_increase_factor_without_path_match = 0.0f;


      /* Initialize customer pre run input*/
      Ced_Init_Input(&ced_input);

      /* Initialization before ced pre run */
      Ced_Pre_Run_Init(&ced_instance);


      object_data          = data.object_data;
      p_vehicle_data       = &data.vehicle_data;
      fbk_output.p_pa_data = &data;

      /* Fill path tracking output */
      Ced_Pre_Run(&ced_instance, &ced_input, &pt_output, &fbk_output);

      /* Run Feature Building Kit Index-ID mapping */
      Fbk_Reset_Index_Id_Lookup_Table(&fbk_lookup_table);
      Fbk_Update_Index_Id_Lookup_Table(&fbk_lookup_table, &data);
      fbk_output.p_index_id_lookup_table = &fbk_lookup_table;


      /* Init ced_core_output */
      Ced_Reset(&ced_core_output, &ced_persistance, &fbk_lookup_table);


      /* Set some vehicle data for all tests */
      p_vehicle_data->host_length        = 5.0f;
      p_vehicle_data->host_width         = 2.0f;
      p_vehicle_data->lane_width         = 3.0f;
      p_vehicle_data->lane_center_offset = 0.0f;
      p_vehicle_data->host_speed         = 0.0f;
      data.time_diff_to_last_cycle       = 0.05f;

      /* Set up default zones for CED */
      Ced_Create_Funnel_Zone(&ced_funnel_zone, p_vehicle_data, &ced_cals);
      Ced_Create_Collision_Zone(&ced_collision_zone, p_vehicle_data, &ced_cals);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }

 public:
   Ced_Object_T Ced_Create_Object_From_Tracker_Output(uint8_t obj_index);

   void Ced_Fill_Raw_Tracker_Output(uint8_t obj_index,
                                    float32_T long_pos,
                                    float32_T lat_pos,
                                    float32_T long_vel,
                                    float32_T lat_vel,
                                    float32_T long_vel_rel,
                                    float32_T lat_vel_rel);
};

/* required helper functions to create ced_objects */
Ced_Object_T Ced_Common_Functions_Test::Ced_Create_Object_From_Tracker_Output(uint8_t obj_index)
{
   Ced_Object_T ced_object{};

   ced_object.tracker_data       = ced_instance.core_input.p_pa_data->object_data[obj_index];
   ced_object.tracker_data.index = obj_index;

   ced_object.attributes.length_predicted = 5.0f;
   ced_object.attributes.width_predicted  = 2.0f;
   ced_object.attributes.direction        = FBK_SIDE_UNDEFINED;

   return ced_object;
}

/* required helper functions to fill raw GDSR tracker output */
void Ced_Common_Functions_Test::Ced_Fill_Raw_Tracker_Output(uint8_t obj_index,
                                                            float32_T long_pos,
                                                            float32_T lat_pos,
                                                            float32_T long_vel,
                                                            float32_T lat_vel,
                                                            float32_T long_vel_rel,
                                                            float32_T lat_vel_rel)
{

   object_data[obj_index].status                = PA_OBJ_STATUS_MATURE;
   object_data[obj_index].id                    = obj_index + 1u;
   object_data[obj_index].age                   = 8u;
   object_data[obj_index].existence_probability = ced_cals.k_ced_object_existence_probability_min;
   object_data[obj_index].width                 = 2.0f;
   object_data[obj_index].length                = 5.0f;
   object_data[obj_index].vcs_pos.x             = long_pos;
   object_data[obj_index].vcs_pos.y             = lat_pos;
   object_data[obj_index].vcs_vel.x             = long_vel;
   object_data[obj_index].vcs_vel.y             = lat_vel;
   object_data[obj_index].vcs_vel_rel.x         = long_vel_rel;
   object_data[obj_index].vcs_vel_rel.y         = lat_vel_rel;
   object_data[obj_index].vcs_accel.x           = 0.0f;
   object_data[obj_index].vcs_accel.y           = 0.0f;
   object_data[obj_index].vcs_heading           = ced_cals.k_ced_object_heading_abs_angle_max;
   object_data[obj_index].heading_rate          = 1.0f;
   object_data[obj_index].f_stationary          = FBK_FALSE;
   object_data[obj_index].f_reflection          = FBK_FALSE;
   object_data[obj_index].f_is_in_rr_sensor_fov = 1u;
   object_data[obj_index].f_is_in_rl_sensor_fov = 1u;
}

#endif /* CED_COMMON_FUNCTIONS_H */
