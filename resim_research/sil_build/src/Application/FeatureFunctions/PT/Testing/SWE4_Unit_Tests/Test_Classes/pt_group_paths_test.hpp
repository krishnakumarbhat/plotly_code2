#ifndef PT_GROUP_PATHS_TEST_HPP
#define PT_GROUP_PATHS_TEST_HPP

/**
 * @file pt_group_paths_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for PT path grouping functions unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "pt_shared_unit_test_constructors.hpp"
#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "pa_const_macros.h"
#include "pa_reuse.h"
#include "pt_constants.h"
#include "pt_core_calibration.h"
#include "pt_input_t.h"
#include "pt_modify_grid_cals.h"
#include "pt_output_t.h"
#include "pt_persistent_t.h"
#include "pt_reset.h"
}

class Pt_Group_Paths_Test : public ::testing::Test, public Pt_Shared_Unit_Test_Constructors
{
 public:
   /**
    * Here is the place for some initializations of public members
    */
   Pt_Core_Calibration_T cals;
   Pt_Persistent_T pt_persistent{};
   Pt_Input_T pt_input{};
   Pt_Output_T p_path_output{};
   Pt_Best_Path_Obj_Pair_Persistent_T best_path_obj_pairs[PT_OBJ_MAX_ARRAY_SIZE]{};

   void SetUp() override
   {

      Pt_Update_Grid_Array_Defaults(pt_input.grid_pt_array);
      Pt_Core_Cal_Update_Defaults(&cals);

      Pt_Set_Num_Grid_Pts_Dependend_Cals(&cals, pt_input.grid_pt_array, &pt_input.Num_Grid_Pts_Dep_Cals);
      Pt_Reset_All_Paths(pt_persistent.paths, &p_path_output, best_path_obj_pairs);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};


#endif /*PT_GROUP_PATHS_TEST_HPP*/
