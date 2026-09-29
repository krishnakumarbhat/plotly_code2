#ifndef PT_RESET_TEST_HPP
#define PT_RESET_TEST_HPP

/**
 * @file pt_reset_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for PT reset functions unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "pt_shared_unit_test_constructors.hpp"
#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_const_macros.h"
#include "pa_reuse.h"
#include "pt_constants.h"
#include "pt_core_calibration.h"
#include "pt_output_t.h"
#include "pt_persistent_t.h"
}

class Pt_Reset_Test : public ::testing::Test, public Pt_Shared_Unit_Test_Constructors
{
 protected:
 public:
   Pt_Core_Calibration_T pt_cals;
   Pt_Persistent_T pt_persistent{};
   Pt_Best_Path_Obj_Pair_Persistent_T best_path_object_pairs[PA_OBJ_NUMBER_OF_OBJECTS]{};
   Pt_Path_T single_path{};
   float32_T p_grid_array[PT_NUM_GRID_POINTS];
   Pt_Output_T path_output{};

   /**
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      Pt_Update_Grid_Array_Defaults(p_grid_array);
      for (uint8_t i = 0; i < PT_NUMBER_OF_PATHS; i++)
      {
         for (uint8_t j = 0; j < PT_NUM_GRID_POINTS; j++)
         {
            pt_persistent.paths[i].path_points[j] = 3;
         }

         pt_persistent.paths[i].obj_curr_used_for_path_build.id  = 3;
         pt_persistent.paths[i].obj_curr_used_for_path_build.age = 3;
         pt_persistent.paths[i].first_p                          = 3;
         pt_persistent.paths[i].last_p                           = 3;
         pt_persistent.paths[i].path_state                       = PATH_STATUS_MATURE;
         pt_persistent.paths[i].direction                        = PATH_DIRECTION_LONG_FORWARD;
         pt_persistent.paths[i].max_speed                        = 3.0f;
         pt_persistent.paths[i].path_age                         = 3;
         pt_persistent.paths[i].first                            = Create_2d_Vector_Origin();
         pt_persistent.paths[i].last_mat                         = Create_2d_Vector_Origin();
         pt_persistent.paths[i].new_path_point_status            = PATH_POINT_NEW_LAST;
         pt_persistent.paths[i].path_border_status               = PATH_BORDER_NEW_LAST;
         pt_persistent.paths[i].path_index                       = i;
      }

      for (uint8_t i = 0; i < PA_OBJ_NUMBER_OF_OBJECTS; i++)
      {

         path_output.path_obj_pair_output[i].track_match                = 3;
         path_output.path_obj_pair_output[i].track_match_last_cycle     = 3;
         path_output.path_obj_pair_output[i].track_match_age            = 3;
         path_output.path_obj_pair_output[i].range_at_zero              = 3.0f;
         path_output.path_obj_pair_output[i].range_at_host_edge         = 3.0f;
         path_output.path_obj_pair_output[i].range_to_current_path_part = 3.0f;
         path_output.path_obj_pair_output[i].length_of_trajectory       = 3.0f;
         path_output.path_obj_pair_output[i].path_heading               = 3.0f;
         path_output.path_obj_pair_output[i].path_direction             = PATH_DIRECTION_LONG_FORWARD;
      }
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};

#endif /* PT_RESET_TEST_HPP */
