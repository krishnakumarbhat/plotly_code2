#ifndef PT_IFACE_TEST_HPP
#define PT_IFACE_TEST_HPP

/**
 * @file pt_iface_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for PT interface functions unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_ego_traj_predictor_instance.h"
#include "fbk_host_trail.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pt_constants.h"
#include "pt_core_calibration.h"
#include "pt_input_t.h"
#include "pt_instance.h"
#include "pt_modify_grid_cals.h"
}

class Pt_Iface_Test : public ::testing::Test
{
 public:
   Pt_Instance_T pt_instance{};
   Pt_Input_T *p_pt_input{};
   Pt_Output_T Pt_Output{};
   Fbk_Output_T fbk_output{};
   const Fbk_Host_Trail_T host_trail{};
   const Fbk_Ego_Traj_Predictor_Instance_T ego_traj_predictor_instance{};
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data     = nullptr;
   Fbk_Vehicle_Data_T *p_vehicle_data = nullptr;

   /**
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      p_pt_input = &pt_instance.pt_core_input;
      Pt_Update_Grid_Array_Defaults(p_pt_input->grid_pt_array);
      Pt_Core_Cal_Update_Defaults(&pt_instance.calibration);

      fbk_output.p_pa_data    = &data;
      fbk_output.p_host_trail = &host_trail;
      object_data             = data.object_data;
      p_vehicle_data          = &(data.vehicle_data);

      Pt_Set_Num_Grid_Pts_Dependend_Cals(&pt_instance.calibration, p_pt_input->grid_pt_array, &p_pt_input->Num_Grid_Pts_Dep_Cals);

      p_pt_input->p_fbk_output = &fbk_output;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};

#endif /* PT_IFACE_TEST_HPP */
