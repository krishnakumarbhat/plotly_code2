#ifndef CTA_OBJECT_VALIDATOR_TEST
#define CTA_OBJECT_VALIDATOR_TEST

/**
 * @file cta_object_validator_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for cta_object_validator module
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "cta_core_calibration.h"
#include "cta_core_input_t.h"
#include "cta_instance.h"
#include "cta_types.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_context.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "pt_output_t.h"
}


/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Cta_Object_Validator_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Cta_Object_Data_T object{};
   Cta_Core_Calibration_T cals;
   Cta_Instance_T cta_instance{};

   Fbk_Object_Data_T tracker_output{};
   Cta_Object_Attributes_T attributes{};
   Cta_Object_Persistent_T persistent{};
   Cta_Core_Input_T core_input{};

   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Pa_Context_T context{};

   Pt_Nearest_Path_T pt_nearest_path{};
   Pt_Path_Object_Pair_Output_T pt_match_info{};

   void SetUp() override
   {
      Cta_Core_Cal_Update_Defaults(&cals);

      object.attributes   = &attributes;
      object.tracker_data = tracker_output;
      object.persistent   = &persistent;

      object.attributes->p_pt_match_info        = &pt_match_info;
      object.attributes->p_pt_nearest_path_info = &pt_nearest_path;
      core_input.p_pa_data                      = &data;

      object_data    = data.object_data;
      p_vehicle_data = &(data.vehicle_data);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

   void Cta_Create_Valid_Object(Cta_Object_Data_T *p_cta_object, Cta_Core_Calibration_T *p_cta_cals);

 protected:
};

void Cta_Object_Validator_Test::Cta_Create_Valid_Object(Cta_Object_Data_T *p_cta_object, Cta_Core_Calibration_T *p_cta_cals)
{
   p_cta_object->tracker_data.f_reflection = FBK_FALSE;
   p_cta_object->tracker_data.status       = PA_OBJ_STATUS_MATURE;
   p_cta_object->tracker_data.stage_age    = p_cta_cals->k_cta_cycles_coasted_to_ignore - 1u;

   p_cta_cals->k_cta_f_apply_path_tracking                 = (uint8_t) FBK_TRUE;
   p_cta_cals->k_cta_f_use_ghost_detector                  = FBK_FALSE;
   p_cta_cals->k_cta_f_use_object_min_object_age_in_cycles = FBK_TRUE;
   p_cta_cals->k_cta_f_check_reflection_signal             = FBK_TRUE;
   p_cta_cals->k_cta_min_speed                             = 0.0f;

   p_cta_object->tracker_data.age                   = p_cta_cals->k_cta_min_object_age_check_valid + 1u;
   p_cta_object->tracker_data.speed                 = 0.5f * (p_cta_cals->k_cta_min_speed + p_cta_cals->k_cta_max_speed);
   p_cta_object->tracker_data.heading_variance      = 0.5f * (0.0f + p_cta_cals->k_cta_max_heading_variance);
   p_cta_object->tracker_data.existence_probability = 1.1f * p_cta_cals->k_cta_min_rel_existence_probability;
   p_cta_object->tracker_data.obstruction_prob      = 0.9f * p_cta_cals->k_cta_max_obstruction_probability;
   p_cta_object->tracker_data.vcs_heading           = -0.5f * PI;
   p_cta_object->tracker_data.index                 = 1u;
   data.object_data[0u].f_is_in_rr_sensor_fov       = FBK_TRUE;

   p_cta_object->persistent->obj_validity_suppression_counter      = p_cta_cals->k_cta_object_supress_counter + (uint8_t) 1;
   p_cta_object->persistent->prev_cycle_crit_level[CTA_MODE_REAR]  = CTA_CRIT_LEVEL_NONE;
   p_cta_object->persistent->prev_cycle_crit_level[CTA_MODE_FRONT] = CTA_CRIT_LEVEL_NONE;

   p_cta_object->attributes->relative_velocity.y = p_cta_cals->k_cta_min_lateral_approach_speed;
   p_cta_object->attributes->approach_side       = FBK_SIDE_LEFT;
   p_cta_object->attributes->CTA_heading     = 0.5f * (p_cta_cals->k_cta_heading_range[0u] + p_cta_cals->k_cta_heading_range[1u]);
   p_cta_object->attributes->p_pt_match_info = &(pt_match_info);
   p_cta_object->attributes->p_pt_nearest_path_info = &(pt_nearest_path);
   pt_match_info.path_heading                       = p_cta_object->tracker_data.vcs_heading;
   pt_match_info.track_match                        = 0;
   pt_match_info.track_match_age                    = p_cta_cals->k_cta_cycles_valid_match_of_pot_ghost;
   pt_nearest_path.track_idx_nearest_path           = 0;
   pt_nearest_path.segment_heading_diff             = 0.0f;
   pt_nearest_path.range_vcs_proj_to_path_segment   = 0.0f;
   p_cta_object->attributes->p_pt_match_info        = &(pt_match_info);
   p_cta_object->attributes->p_pt_nearest_path_info = &(pt_nearest_path);
}

#endif /* CTA_OBJECT_VALIDATOR_TEST */
