#ifndef PT_PATH_ROTATION_TEST_HPP
#define PT_PATH_ROTATION_TEST_HPP

/**
 * @file pt_path_rotation_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for PT path rotation functions unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "pt_shared_unit_test_constructors.hpp"
#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "pa_const_macros.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pt_constants.h"
#include "pt_core_calibration.h"
#include "pt_input_t.h"
#include "pt_output_t.h"
#include "pt_persistent_t.h"
#include "pt_reset.h"
}

class Pt_Path_Rotation_Test : public ::testing::Test, public Pt_Shared_Unit_Test_Constructors
{
 protected:
   /**
    * Here is the place for some initializations of public members
    */

   Pa_Data_T data{};
   Fbk_Output_T fbk_output{};
   Pt_Input_T pt_input{};
   Pt_Core_Calibration_T cals;

   const Fbk_Object_Data_T *object_data = nullptr;
   Fbk_Vehicle_Data_T *p_vehicle_data   = nullptr;

   Pt_Path_T path{};
   Pt_Path_T path_for_temp_path_init{};
   Pt_Persistent_T pt_persistent{};
   float32_T *p_grid_array;
   float32_T default_slope{};
   float32_T default_offset{};
   Pt_Output_T path_output[PA_OBJ_NUMBER_OF_OBJECTS]{};

   void SetUp() override
   {
      p_grid_array = pt_input.grid_pt_array;
      Pt_Update_Grid_Array_Defaults(p_grid_array);
      fbk_output.p_pa_data  = &data;
      pt_input.p_fbk_output = &fbk_output;
      object_data           = data.object_data;
      p_vehicle_data        = &data.vehicle_data;

      p_vehicle_data->rear_axle_position = 0.0f;
      data.time_diff_to_last_cycle       = 0.04f;
      default_slope                      = 0.1f;
      default_offset                     = 2.0f;

      /*Setup pt_persistent.paths*/
      for (uint8_t idx = PT_LOWEST_GRID_POINT_INDEX; idx <= PT_HIGHEST_GRID_POINT_INDEX; idx++)
      {
         path.path_points[idx]                    = 0.0f;
         path_for_temp_path_init.path_points[idx] = 15.0f;
      }

      Pt_Reset_All_Paths(pt_persistent.paths, path_output, pt_persistent.best_path_obj_pairs);

      Pt_Core_Cal_Update_Defaults(&cals);
   }


   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};


#endif /* PT_PATH_ROTATION_TEST_HPP */