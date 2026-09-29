#ifndef LCDA_TEST_HPP
#define LCDA_TEST_HPP

/**
 * @file lcda_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class Lcda_Test
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest.h"
#include <assert.h>

extern "C"
{
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "lcda.h"
#include "lcda_input_generator.h"
#include "pa_data.h"
}


/**
 * Class used to create a fixture for lcda_tests.c
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Lcda_Test : public ::testing::Test
{
 protected:
   Lcda_Instance_T lcda_instance{};
   Pa_Data_T data{};
   Fbk_Index_Id_Lookup_Table_T lookup_table{};

   Lcda_Core_Calibration_T &lcda_cals   = lcda_instance.calibration;
   Fbk_Object_Data_T *object_data       = data.object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data   = &data.vehicle_data;
   Lcda_Core_Input_T &lcda_core_input   = lcda_instance.core_input;
   Lcda_Core_Output_T &lcda_core_output = lcda_instance.core_output;
   Lcda_Persistent_T &lcda_persistent   = lcda_instance.persistent;

   Fbk_Output_T fbk_output{};

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp()
   {
      /* Update default cals and set calibration pointer */
      Lcda_Core_Cal_Update_Defaults(&lcda_cals);

      lcda_cals.k_lcda_enable                        = 1u;
      lcda_cals.k_lcda_enable_via_cal                = 0u;
      lcda_cals.k_bsw_enable                         = 1u;
      lcda_cals.k_bsw_enable_via_cal                 = 0u;
      lcda_cals.k_cvw_enable                         = 1u;
      lcda_cals.k_cvw_enable_via_cal                 = 0u;
      lcda_cals.k_slc_enable                         = 1u;
      lcda_cals.k_slc_enable_via_cal                 = 0u;
      lcda_cals.k_lcda_host_activation_speed_min     = 2.0f;
      lcda_cals.k_lcda_host_activation_speed_min_hys = 0.5f;

      // so that LCDA is not deactivated due to small curve radius
      // lcda_cals.k_lcda_disable_due_to_small_curve_radius = 0u;

      /* Set basic vehicle data */
      p_vehicle_data->host_speed         = 2.0f * lcda_cals.k_lcda_host_activation_speed_min;
      p_vehicle_data->host_length        = 4.5f;
      p_vehicle_data->host_width         = 2.0f;
      p_vehicle_data->lane_width         = 3.0f;
      p_vehicle_data->lane_center_offset = 0.0f;

      // initialize LCDA
      Lcda_Reset(&lcda_instance, &data);

      fbk_output.p_pa_data               = &data;
      fbk_output.p_index_id_lookup_table = &lookup_table;

      lcda_core_input.p_pa_data = &data;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }
};

#endif /* LCDA_TEST_HPP */
