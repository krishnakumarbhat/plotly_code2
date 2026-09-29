#ifndef SCW_TEST_HPP
#define SCW_TEST_HPP

/**
 * @file scw_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for SCW unit tests
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_field_of_interest.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "pa_const_macros.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "scw_core_calibration.h"
#include "scw_core_input_t.h"
#include "scw_core_output_t.h"
#include "scw_iface.h"
#include "scw_instance_t.h"
#include "scw_output_t.h"
#include "scw_persistent_t.h"
}

/**
 * Class used to create a fixture for SCW Tests
 * For writing two or more tests that operate on similar data, we can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Scw_Test : public ::testing::Test
{
 public:
   Scw_Instance_T scw_instance{};
   Scw_Persistent_T *p_scw_persistent;
   Scw_Core_Calibration_T *p_scw_calibration;
   Scw_Core_Input_T *p_scw_core_input;
   Scw_Core_Output_T *p_scw_core_output;
   Scw_Input_T scw_input{};
   Scw_Output_T scw_output{};
   Pa_Data_T pa_data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Fbk_Field_Of_Interest_T res_foi{};
   Fbk_Field_Of_Interest_T scw_zone;
   Fbk_Field_Of_Interest_T scw_hysteresis_zone;

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      uint8_t idx;

      p_scw_persistent  = &(scw_instance.persistent);
      p_scw_calibration = &(scw_instance.calibration);
      p_scw_core_input  = &(scw_instance.core_input);
      p_scw_core_output = &(scw_instance.core_output);

      object_data    = pa_data.object_data;
      p_vehicle_data = &(pa_data.vehicle_data);

      /* Initialize core input */
      p_scw_core_input->p_pa_data = &pa_data;

      /* Initialize all tracks to invalid */
      for (idx = 0; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
      {
         object_data[idx].status = PA_OBJ_STATUS_INVALID;
      }

      /* Initialize calibration values */
      Scw_Core_Cal_Update_Defaults(p_scw_calibration);

      p_vehicle_data->host_width  = 2.0f;
      p_vehicle_data->host_length = 5.0f;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /* SCW_TEST_HPP */
