#ifndef LCDA_CREATE_CVW_ZONE_TEST_HPP
#define LCDA_CREATE_CVW_ZONE_TEST_HPP

/**
 * @file lcda_create_cvw_zone_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class Lcda_Create_Cvw_Zone_Test
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_core_calibration.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_instance.h"
#include "lcda_types.h"
#include "pa_data.h"
#include "pa_reuse.h"
}

/**
 * Class used to create a fixture for LCDA_ProcessCvw_test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Lcda_Create_Cvw_Zone_Test : public ::testing::Test
{
 protected:
   Lcda_Instance_T lcda_instance{};
   Pa_Data_T data{};

   Lcda_Core_Calibration_T &lcda_cals    = lcda_instance.calibration;
   Fbk_Object_Data_T *object_data        = data.object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data    = &data.vehicle_data;
   Lcda_Core_Input_T &lcda_core_input    = lcda_instance.core_input;
   Lcda_Cvw_Persistent_T &cvw_persistent = lcda_instance.cvw_persistent;

   Cvw_Object_T cvw_object{};
   Fbk_Object_Data_T tracker_object{};


   boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES];

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp()
   {
      /* Update default cals and set calibration pointer */
      Lcda_Core_Cal_Update_Defaults(&lcda_cals);

      cvw_object.p_tracker_data = &tracker_object;

      /* Set basic vehicle data */
      p_vehicle_data->host_length        = 4.5f;
      p_vehicle_data->host_width         = 2.0f;
      p_vehicle_data->lane_width         = 3.0f;
      p_vehicle_data->lane_center_offset = 0.0f;

      /* Initialize core input */
      lcda_core_input.lane_width         = p_vehicle_data->lane_width;
      lcda_core_input.lane_center_offset = p_vehicle_data->lane_center_offset;
      lcda_core_input.p_pa_data          = &data;

      f_use_small_lc_intention_zone[FBK_SIDE_LEFT]  = FBK_FALSE;
      f_use_small_lc_intention_zone[FBK_SIDE_RIGHT] = FBK_FALSE;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }
};

#endif /* CREATE_CVWZONE_TEST_H */
