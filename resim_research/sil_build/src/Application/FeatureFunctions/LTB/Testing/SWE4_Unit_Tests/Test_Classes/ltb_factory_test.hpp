#ifndef LTB_FACTORY_TEST_H
#define LTB_FACTORY_TEST_H

/**
 * @file ltb_create_zones_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for ltb_factory unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "ltb_core_calibration.h"
#include "pa_data.h"
}

/**
 * Class used to create a fixture for LTB test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ltb_Factory_Test : public ::testing::Test
{
 protected:
   Ltb_Core_Calibration_T ltb_cals;

   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp()
   {
      /* Init cals */
      Ltb_Core_Cal_Update_Defaults(&ltb_cals);
      ltb_cals.k_ltb_zone_length = 60.0f;
      ltb_cals.k_ltb_zone_width  = 20.0f;

      /* Set contexts vehicle and tracker output pointer */
      object_data    = data.object_data;
      p_vehicle_data = &data.vehicle_data;

      /* Set some vehicle data for all tests */
      p_vehicle_data->host_length        = 5.0f;
      p_vehicle_data->host_width         = 2.0f;
      p_vehicle_data->lane_width         = 3.0f;
      p_vehicle_data->lane_center_offset = 0.0f;
      p_vehicle_data->host_speed         = 0.0f;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }

 public:
};

#endif /* LTB_FACTORY_TEST_H */