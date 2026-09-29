#ifndef LCDA_CREATE_BSW_ZONE_TEST_HPP
#define LCDA_CREATE_BSW_ZONE_TEST_HPP

/**
 * @file lcda_create_bsw_zone_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class Lcda_Create_Bsw_Zone_Test
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_field_of_interest.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "lcda.h"
#include "lcda_core_calibration.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_input_generator.h"
#include "lcda_types.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
}

/**
 * Class used to create a fixture for LCDA_ProcessBsw_test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Lcda_Create_Bsw_Zone_Test : public ::testing::Test
{
 protected:
   Lcda_Instance_T lcda_instance{};
   Pa_Data_T data{};

   Lcda_Core_Calibration_T &lcda_cals      = lcda_instance.calibration;
   Fbk_Object_Data_T *object_data          = data.object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data      = &data.vehicle_data;
   Lcda_Core_Input_T &lcda_core_input      = lcda_instance.core_input;
   Lcda_Bsw_Core_Output_T &slc_core_output = lcda_instance.core_output.bsw_core_output;
   Lcda_Bsw_Persistent_T &bsw_persistent   = lcda_instance.bsw_persistent;

   Bsw_Object_T bsw_object{};
   Fbk_Object_Data_T tracker_object{};

   Fbk_Field_Of_Interest_T default_bsw_zone_hys{};
   Fbk_Field_Of_Interest_T default_bsw_zone{};

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp()
   {
      Vector_2d_T zone_p0; // Point zero for zone creation

      /* Update default cals and set calibration pointer */
      Lcda_Core_Cal_Update_Defaults(&lcda_cals);

      Lcda_Reset(&lcda_instance, &data);

      bsw_object.p_tracker_data = &tracker_object;

      /* Set basic vehicle data */
      p_vehicle_data->host_length        = 4.5f;
      p_vehicle_data->host_width         = 2.0f;
      p_vehicle_data->lane_width         = 3.0f;
      p_vehicle_data->lane_center_offset = 0.0f;

      /* Initialize core input */
      lcda_core_input.lane_width         = p_vehicle_data->lane_width;
      lcda_core_input.lane_center_offset = p_vehicle_data->lane_center_offset;
      lcda_core_input.p_pa_data          = &data;

      /* Create the default bsw zone (right of ego) which is a rectangle
       * point 0 = (-2.0m, 2.5m)
       * zone length = 10m, width = 2m
       */
      zone_p0.x = -2.0f;
      zone_p0.y = 2.5f;
      Lcda_Create_Zone(zone_p0, 10.0f, 2.0f, &default_bsw_zone);

      /* Create the default bsw hys zone (right side of ego)
       * point 0 = (-0.5m, 3.0m)
       * zone length = 7m, width = 3m
       */
      zone_p0.x = -0.5f;
      zone_p0.y = 2.5f;
      Lcda_Create_Zone(zone_p0, 7.0f, 3.0f, &default_bsw_zone_hys);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }
};

#endif /* LCDA_CREATE_BSW_ZONE_TEST_HPP */
