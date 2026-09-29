#ifndef PT_HOST_LANE_TEST_H
#define PT_HOST_LANE_TEST_H

/**
 * @file pt_host_lane_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for pt_host_lane unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "pt_shared_unit_test_constructors.hpp"
#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_iface_types.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pt_constants.h"
#include "pt_core_calibration.h"
#include "pt_input_t.h"
#include "pt_persistent_t.h"
}

/**
 * Class used to create a fixture for pt_host_lane_test.c
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Pt_Host_Lane_Test : public ::testing::Test, public Pt_Shared_Unit_Test_Constructors
{
 public:
   Pt_Core_Calibration_T cals;
   Pt_Persistent_T pt_persistent{};
   Vector_2d_T trail_vcs[FBK_NUM_HOST_TRAIL_POINTS]{};
   Fbk_Host_Trail_T host_trail{};
   Pt_Input_T path_tracking_input{};
   Fbk_Output_T fbk_output{};
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data     = nullptr;
   Fbk_Vehicle_Data_T *p_vehicle_data = nullptr;

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp()
   {
      Pt_Core_Cal_Update_Defaults(&cals);
      Pt_Update_Grid_Array_Defaults(path_tracking_input.grid_pt_array);

      fbk_output.p_pa_data = &data;
      object_data          = data.object_data;
      p_vehicle_data       = &(data.vehicle_data);

      path_tracking_input.p_fbk_output = &fbk_output;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }

 protected:
};

#endif /* PT_HOST_LANE_TEST_H */
