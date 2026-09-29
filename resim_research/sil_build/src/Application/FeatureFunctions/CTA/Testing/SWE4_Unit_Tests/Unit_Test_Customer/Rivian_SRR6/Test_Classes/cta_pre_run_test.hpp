#ifndef CTA_PRE_RUN_TEST
#define CTA_PRE_RUN_TEST

/**
 * @file cta_pre_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for rivian srr6 pre run
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

#include "gtest/gtest_pred_impl.h"


extern "C"
{
#include "cta_core_calibration.h"
#include "cta_core_input_t.h"
#include "cta_input_t.h"
#include "cta_instance.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "pa_context.h"
#include "pt_output_t.h"
}


/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Cta_Pre_Run_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Cta_Core_Calibration_T *p_cta_cals = nullptr;
   Fbk_Object_Data_T *object_data     = nullptr;
   Fbk_Vehicle_Data_T *p_vehicle_data = nullptr;
   Cta_Core_Input_T *p_cta_core_input = nullptr;
   Cta_Input_T cta_input{};
   Pt_Output_T pt_output{};

   Pa_Data_T data{};
   Cta_Instance_T cta_instance{};
   Fbk_Output_T fbk_output{};

   void SetUp() override
   {
      p_cta_cals = &cta_instance.calibration;
      Cta_Core_Cal_Update_Defaults(p_cta_cals);
      fbk_output.p_pa_data              = &data;
      object_data                       = data.object_data;
      p_vehicle_data                    = &(data.vehicle_data);
      cta_instance.core_input.p_pa_data = &data;

      cta_instance.calibration.k_cta_max_long_point_criticality_level[0][0] = -0.5f;
      cta_instance.calibration.k_cta_max_long_point_criticality_level[0][1] = -0.5f;
      cta_instance.calibration.k_cta_max_long_point_criticality_level[1][0] = 7.0f;
      cta_instance.calibration.k_cta_max_long_point_criticality_level[1][1] = 7.0f;
      cta_instance.calibration.k_cta_min_long_point_criticality_level[0][0] = 7.0f;
      cta_instance.calibration.k_cta_min_long_point_criticality_level[0][1] = 7.0f;
      cta_instance.calibration.k_cta_min_long_point_criticality_level[1][0] = -0.5f;
      cta_instance.calibration.k_cta_min_long_point_criticality_level[1][1] = -0.5f;
      cta_instance.calibration.k_cta_max_length_fov                         = 25.0f;
      cta_instance.calibration.k_cta_angles_zone_definition[0]              = 0.43f;
      cta_instance.calibration.k_cta_angles_zone_definition[1]              = 0.43f;
      p_vehicle_data->host_length                                           = 4.65f;
      p_vehicle_data->host_width                                            = 1.83f;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};


#endif /* CTA_PRE_RUN_TEST */