#ifndef PT_OBJECT_MATCHING_TEST_HPP
#define PT_OBJECT_MATCHING_TEST_HPP

/**
 * @file pt_object_matching_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for PT object matching functions unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "pt_shared_unit_test_constructors.hpp"
#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pt_constants.h"
#include "pt_core_calibration.h"
#include "pt_directions.h"
#include "pt_input_t.h"
#include "pt_modify_grid_cals.h"
#include "pt_output_t.h"
#include "pt_persistent_t.h"
#include "pt_types.h"
}


class Pt_Object_Matching_Test : public ::testing::Test, public Pt_Shared_Unit_Test_Constructors
{
 public:
   /**
    * Here is the place for some initializations of public members
    */
   Pa_Data_T data{};
   Fbk_Output_T fbk_output{};
   Pt_Input_T pt_input{};
   Pt_Core_Calibration_T cals;
   Pt_Output_T path_output{};
   Pt_Persistent_T pt_persistent{};

   Fbk_Object_Data_T *object_data           = nullptr;
   const Fbk_Vehicle_Data_T *p_vehicle_data = nullptr;
   float32_T *p_grid_array                  = nullptr;

   Pt_Object_T object{};

   Pt_Path_Obj_Pair_Consumer_Info_T path_obj_pair_consumer_info{};
   Pt_Best_Path_Obj_Pair_Persistent_T best_path_object_pair{};
   Pt_Path_Obj_Pair_Info_T path_obj_info{};
   Pt_Path_Obj_Pair_Confidence_T path_obj_pair_confidence{};
   Pt_Object_Mov_Direction_T obj_move_dir{};
   Pt_Path_T path{};
   uint8_t next_point_index{};

   void SetUp() override
   {
      p_grid_array = pt_input.grid_pt_array;
      Pt_Update_Grid_Array_Defaults(p_grid_array);
      Pt_Core_Cal_Update_Defaults(&cals);

      Pt_Set_Num_Grid_Pts_Dependend_Cals(&cals, pt_input.grid_pt_array, &pt_input.Num_Grid_Pts_Dep_Cals);

      fbk_output.p_pa_data  = &data;
      pt_input.p_fbk_output = &fbk_output;
      object_data           = data.object_data;
      p_vehicle_data        = &data.vehicle_data;
   }


   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};

#endif
