#ifndef LTB_TEST_H
#define LTB_TEST_H

/**
 * @file ltb_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for LTB unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest.h> // IWYU pragma: keep

extern "C"
{
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "fbk_vehicle_data_t.h"
#include "ltb.h" // IWYU pragma: keep
#include "ltb_core_calibration.h"
#include "ltb_core_input_t.h"
#include "ltb_core_output_t.h"
#include "ltb_factory.h"
#include "ltb_input_t.h"
#include "ltb_instance.h"
#include "ltb_persistent_t.h"
#include "ltb_pre_run.h"
#include "ltb_types.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}

/**
 * Class used to create a fixture for LTB test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ltb_Test : public ::testing::Test
{
 protected:
   Ltb_Core_Calibration_T *p_ltb_cals;
   Ltb_Persistent_T *p_ltb_persistent;
   Ltb_Instance_T ltb_instance{};
   Ltb_Core_Input_T ltb_core_input{};
   Ltb_Core_Output_T ltb_core_output{};
   Ltb_Input_T ltb_input{};
   Ltb_Object_T ltb_object{};
   Fbk_Output_T fbk_output{};

   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;

   uint8_t object_index;

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp()
   {
      p_ltb_persistent = &ltb_instance.persistent;
      p_ltb_cals       = &ltb_instance.calibration;

      /* Init cals */
      Ltb_Core_Cal_Update_Defaults(p_ltb_cals);

      /* Set ltb_input context */
      ltb_core_input.p_pa_data = &data;

      /* Set contexts vehicle and tracker output pointer */
      object_data                       = data.object_data;
      p_vehicle_data                    = &(data.vehicle_data);
      fbk_output.p_pa_data              = &data;
      ltb_instance.core_input.p_pa_data = &data;

      Ltb_Pre_Run(&ltb_instance, &ltb_input, &fbk_output);

      /* Init ltb_core_output */
      Ltb_Reset(&ltb_core_output, p_ltb_cals, p_ltb_persistent);

      /* Set some vehicle data for all tests */
      p_vehicle_data->host_length        = 5.0f;
      p_vehicle_data->host_width         = 2.0f;
      p_vehicle_data->lane_width         = 3.0f;
      p_vehicle_data->lane_center_offset = 0.0f;
      p_vehicle_data->host_speed         = 0.0f;
      data.time_diff_to_last_cycle       = 0.05f;

      ltb_core_input.f_ltb_enable = FBK_TRUE;

      object_index = 0;

      object_data[object_index].id          = object_index + 1;
      object_data[object_index].status      = PA_OBJ_STATUS_MATURE;
      object_data[object_index].vcs_pos.x   = 10.0f;
      object_data[object_index].vcs_pos.y   = 10.0f;
      object_data[object_index].curvi_pos.x = 10.0f;
      object_data[object_index].curvi_pos.y = 10.0f;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }

 public:
   void Ltb_Fill_Raw_Tracker_Output(uint8_t obj_index,
                                    float32_T long_pos,
                                    float32_T lat_pos,
                                    float32_T long_vel,
                                    float32_T lat_vel,
                                    float32_T long_vel_rel,
                                    float32_T lat_vel_rel);
};

/* required helper functions to fill raw GDSR tracker output */
void Ltb_Test::Ltb_Fill_Raw_Tracker_Output(uint8_t obj_index,
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
   object_data[obj_index].heading_rate          = 1.0f;
   object_data[obj_index].f_stationary          = FBK_FALSE;
   object_data[obj_index].f_reflection          = FBK_FALSE;
   object_data[obj_index].f_is_in_rr_sensor_fov = 1u;
   object_data[obj_index].f_is_in_rl_sensor_fov = 1u;
}

#endif /* LTB_TEST_H */