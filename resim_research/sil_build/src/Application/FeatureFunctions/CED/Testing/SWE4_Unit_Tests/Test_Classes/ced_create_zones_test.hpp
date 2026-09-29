#ifndef CED_CREATE_ZONES_TEST_H
#define CED_CREATE_ZONES_TEST_H

/**
 * @file ced_create_zones_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for ced_create_zones unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "ced_core_calibration.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "pa_data.h"
}

/**
 * Class used to create a fixture for CED test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ced_Create_Zones_Test : public ::testing::Test
{
 protected:
   Ced_Core_Calibration_T ced_cals;

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
      Ced_Core_Cal_Update_Defaults(&ced_cals);
      ced_cals.k_ced_funnel_zone_length            = 60.0f;
      ced_cals.k_ced_funnel_zone_width             = 20.0f;
      ced_cals.k_ced_collision_zone_width          = 1.5f;
      ced_cals.k_ced_ego_lane_width                = 1.0f;
      ced_cals.k_ced_f_path_tracking_enable        = FBK_FALSE;
      ced_cals.k_ced_f_second_warning_level_enable = FBK_TRUE;

      /* Set contexts vehicle and tracker output pointer */
      object_data    = data.object_data;
      p_vehicle_data = &(data.vehicle_data);

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

#endif /* CED_CREATE_ZONES_TEST_H */
