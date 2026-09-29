#ifndef ESA_CREATE_ZONE_TEST_HPP
#define ESA_CREATE_ZONE_TEST_HPP

/**
 * @file esa_create_zone_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class Esa_Create_Zone_Test
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "esa.h"
#include "esa_core_calibration.h"
#include "esa_core_calibration_t.h"
#include "esa_core_input_t.h"
#include "esa_core_output_t.h"
#include "esa_instance_t.h"
#include "esa_types.h"
#include "fbk_field_of_interest.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
}

/**
 * Class used to create a fixture for ESA_Process_test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Esa_Create_Zone_Test : public ::testing::Test
{
 protected:
   Esa_Instance_T esa_instance{};
   Esa_Core_Calibration_T *p_esa_calibration;
   Esa_Persistent_T *p_esa_persistent;
   Pa_Data_T pa_data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Esa_Core_Input_T *p_esa_core_input;
   Esa_Core_Output_T *p_esa_core_output;
   Fbk_Field_Of_Interest_T default_esa_zone_hys{};
   Fbk_Field_Of_Interest_T default_esa_zone{};
   Esa_Object_T esa_object{};
   Fbk_Object_Data_T tracker_object{};

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      p_esa_calibration = &(esa_instance.calibration);
      p_esa_persistent  = &(esa_instance.persistent);
      p_esa_core_input  = &(esa_instance.core_input);
      p_esa_core_output = &(esa_instance.core_output);

      object_data    = pa_data.object_data;
      p_vehicle_data = &(pa_data.vehicle_data);

      p_esa_core_input->p_pa_data = &pa_data;

      /* Update default cals and set calibration pointer */
      Esa_Core_Cal_Update_Defaults(p_esa_calibration);

      Esa_Reset(p_esa_core_input, p_esa_core_output, p_esa_persistent);

      esa_object.p_tracker_data = &tracker_object;

      /* Set basic vehicle data */
      p_vehicle_data->host_length        = 4.5f;
      p_vehicle_data->host_width         = 2.0f;
      p_vehicle_data->lane_width         = 3.0f;
      p_vehicle_data->lane_center_offset = 0.0f;

      /* Initialize core input */
      p_esa_core_input->lane_width         = p_vehicle_data->lane_width;
      p_esa_core_input->lane_center_offset = p_vehicle_data->lane_center_offset;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /* ESA_CREATE_ZONE_TEST_HPP */
