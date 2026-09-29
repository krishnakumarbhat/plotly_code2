#ifndef RECW_OBJECT_VALIDATOR_TEST_HPP
#define RECW_OBJECT_VALIDATOR_TEST_HPP

/**
 * @file recw_object_validator_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for Recw object validation unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

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
#include "recw_core_calibration.h"
#include "recw_core_input_t.h"
#include "recw_persistent_t.h"
#include "recw_types.h"
}

/**
 * Class used to create a fixture for RECW test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Recw_Object_Validator_Test : public ::testing::Test
{
 public:
   Recw_Persistent_T recw_pers{};
   Recw_Core_Calibration_T recw_cals;
   Recw_Core_Input_T recw_core_input{};
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Recw_Object_T recw_obj{};

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      /* Initialize calibration values */
      Recw_Core_Cal_Update_Defaults(&recw_cals);

      /* Initialize context data */
      recw_core_input.p_pa_data = &data;
      object_data               = data.object_data;
      p_vehicle_data            = &(data.vehicle_data);

      /* Initialize core input */
      recw_core_input.f_enable_recw = FBK_TRUE;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

   void Recw_Create_Valid_Object(Recw_Object_T *p_recw_obj, Recw_Core_Calibration_T *p_cals, uint8_t obj_idx);
};

void Recw_Object_Validator_Test::Recw_Create_Valid_Object(Recw_Object_T *p_recw_obj, Recw_Core_Calibration_T *p_cals, uint8_t obj_idx)
{

   p_recw_obj->tracker_data.index                 = obj_idx;
   p_recw_obj->tracker_data.id                    = obj_idx + 1;
   p_recw_obj->tracker_data.existence_probability = 1.1f * p_cals->k_recw_min_existence_prob[RECW_INDEX_ALERT_LEVEL_1];
   p_recw_obj->tracker_data.vcs_vel_rel.x         = p_cals->k_recw_min_rel_velocity[RECW_INDEX_ALERT_LEVEL_1] + EPSILON;
   p_recw_obj->attributes.effective_rel_vel.x =
      p_cals->k_recw_max_allowed_rel_vel_long_diff + 0.9f * p_recw_obj->tracker_data.vcs_vel_rel.x;
   p_recw_obj->tracker_data.width     = 0.9f * p_cals->k_recw_max_object_width_warn_on;
   p_recw_obj->tracker_data.status    = PA_OBJ_STATUS_MATURE;
   p_recw_obj->tracker_data.speed     = 1.1f * p_cals->k_recw_min_speed_not_stationary;
   p_recw_obj->tracker_data.vcs_pos.x = -10.0f;
   p_recw_obj->tracker_data.length    = 5.0f;

   recw_pers.object_data[p_recw_obj->tracker_data.id].age = p_cals->k_recw_min_object_age + 1u;

   p_cals->k_recw_f_apply_lane_filter                    = 0u;
   p_cals->k_recw_f_enable_traffic_light_ghost_detection = 0u;
}


#endif /*RECW_OBJECT_VALIDATOR_TEST_HPP*/
