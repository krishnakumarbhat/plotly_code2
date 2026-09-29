#ifndef TA_TEST_HPP
#define TA_TEST_HPP

/**
 * @file ta_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for TA unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */


#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "fbk_ego_traj_predictor_instance.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "ta_core_calibration.h"
#include "ta_core_input_t.h"
#include "ta_core_output_t.h"
#include "ta_output_t.h"
#include "ta_persistent_t.h"
#include "ta_types.h"
}

/**
 * Class used to create a fixture for TA test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ta_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data     = nullptr;
   Fbk_Vehicle_Data_T *p_vehicle_data = nullptr;
   Fbk_Ego_Traj_Predictor_Instance_T ego_traj_predictor_instance{};

   Ta_Object_T ta_object{};

   Ta_Output_T ta_output{};
   Ta_Core_Input_T ta_core_input{};
   Ta_Core_Output_T ta_core_output{};

   Ta_Persistent_T ta_persistent{};
   Ta_Core_Calibration_T ta_cal;


   uint8_t object_index;

   void SetUp() override
   {
      Ta_Core_Cal_Update_Defaults(&ta_cal);

      /* Initialize context data */
      object_data    = data.object_data;
      p_vehicle_data = &(data.vehicle_data);

      ta_core_input.p_pa_data    = &data;
      ta_core_input.f_fta_enable = FBK_TRUE;
      ta_core_input.f_rta_enable = FBK_TRUE;

      object_index = 0;

      object_data[object_index].id          = object_index + 1;
      object_data[object_index].status      = PA_OBJ_STATUS_MATURE;
      object_data[object_index].vcs_pos.x   = 10.0f;
      object_data[object_index].vcs_pos.y   = 10.0f;
      object_data[object_index].curvi_pos.x = 10.0f;
      object_data[object_index].curvi_pos.y = 10.0f;

      ta_persistent.ta_side_id_prev_cycle[FBK_SIDE_LEFT]     = PA_INVALID_OBJ_ID;
      ta_persistent.ta_side_id_prev_cycle[FBK_SIDE_RIGHT]    = PA_INVALID_OBJ_ID;
      ta_persistent.ta_side_index_prev_cycle[FBK_SIDE_LEFT]  = PA_INVALID_OBJ_INDEX;
      ta_persistent.ta_side_index_prev_cycle[FBK_SIDE_RIGHT] = PA_INVALID_OBJ_INDEX;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};
#endif /*TA_TEST_HPP*/
