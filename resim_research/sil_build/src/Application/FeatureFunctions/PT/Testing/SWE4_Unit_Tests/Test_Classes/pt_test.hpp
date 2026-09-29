#ifndef PT_TEST_HPP
#define PT_TEST_HPP

/**
 * @file pt_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for PT path builder functions unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

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
#include "pt_persistent_t.h"
#include "pt_shared_unit_test_constructors.hpp"
#include "pt_types.h"
}

class Pt_Test : public ::testing::Test, public Pt_Shared_Unit_Test_Constructors
{
 public:
   Pt_Input_T pt_input{};
   Pt_Object_T object{};
   Pt_Core_Calibration_T cals;
   Pt_Object_Orientation_T object_orientation{};
   Pt_Path_T path{};
   Pt_Persistent_T pt_persistent{};
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data     = nullptr;
   Fbk_Vehicle_Data_T *p_vehicle_data = nullptr;
   Fbk_Output_T fbk_output{};

   /**
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      Pt_Core_Cal_Update_Defaults(&cals);

      fbk_output.p_pa_data  = &data;
      object_data           = data.object_data;
      p_vehicle_data        = &(data.vehicle_data);
      pt_input.p_fbk_output = &fbk_output;

      Pt_Update_Grid_Array_Defaults(pt_input.grid_pt_array);
   }


   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};

#endif /* PT_TEST_HPP */
