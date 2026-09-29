#ifndef LCDA_PROCESS_CVW_TEST_HPP
#define LCDA_PROCESS_CVW_TEST_HPP

/**
 * @file lcda_process_cvw_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class Lcda_Process_Cvw_Test
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
#include "lcda_persistent_t.h"
#include "lcda_process_cvw.h" // IWYU pragma: keep
#include "lcda_types.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
}

/**
 * Class used to create a fixture for LCDA_ProcessCvw_test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Lcda_Process_Cvw_Test : public ::testing::Test
{
 protected:
   Lcda_Instance_T lcda_instance{};
   Pa_Data_T data{};
   Fbk_Field_Of_Interest_T default_cvw_zone_hys{};
   Fbk_Field_Of_Interest_T default_cvw_zone{};
   Fbk_Index_Id_Lookup_Table_T lookup_table{};

   Lcda_Core_Calibration_T &lcda_cals      = lcda_instance.calibration;
   Fbk_Object_Data_T *object_data          = data.object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data      = &data.vehicle_data;
   Lcda_Core_Input_T &lcda_core_input      = lcda_instance.core_input;
   Lcda_Cvw_Core_Output_T &cvw_core_output = lcda_instance.core_output.cvw_core_output;
   Lcda_Persistent_T &lcda_persistent      = lcda_instance.persistent;
   Lcda_Cvw_Persistent_T &cvw_persistent   = lcda_instance.cvw_persistent;

   Cvw_Object_T cvw_object{};
   Fbk_Object_Data_T tracker_object{};
   Fbk_Output_T fbk_output{};

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp()
   {
      Vector_2d_T zone_p0; // Point zero for zone creation

      /* Update default cals and set calibration pointer */
      Lcda_Core_Cal_Update_Defaults(&lcda_cals);

      /* Reset outputs and persistence from previous tests */
      Lcda_Reset(&lcda_instance, &data);

      lcda_persistent.turn_signal_held = TURN_SIGNAL_NONE;
      Lcda_Reset_Cvw_Core(&cvw_core_output, &lcda_persistent, &cvw_persistent);

      cvw_object.p_tracker_data = &tracker_object;

      /* Create the default cvw zone (right of ego) which is a rectangle
       * point 0 = (-2.0m, 2.5m)
       * zone length = 30m, width = 4m
       */
      zone_p0.x = -2.0f;
      zone_p0.y = 2.5f;
      Lcda_Create_Zone(zone_p0, 30.0f, 4.0f, &default_cvw_zone);

      /* Create the default cvw hys zone (right side of ego)
       * point 0 = (-0.5m, 3.0m)
       * zone length = 30m, width = 5m
       */
      zone_p0.x = -0.5f;
      zone_p0.y = 2.5f;
      Lcda_Create_Zone(zone_p0, 30.0f, 5.0f, &default_cvw_zone_hys);

      /* Set basic vehicle data */
      p_vehicle_data->host_length        = 4.5f;
      p_vehicle_data->host_width         = 2.0f;
      p_vehicle_data->lane_width         = 3.0f;
      p_vehicle_data->lane_center_offset = 0.0f;

      /* Initialize core input */
      lcda_core_input.enabled_flags.f_lcda_enabled        = 1;
      lcda_core_input.initial_cvw_zone                    = default_cvw_zone;
      lcda_core_input.initial_cvw_zone_hys                = default_cvw_zone_hys;
      lcda_core_input.lane_width                          = p_vehicle_data->lane_width;
      lcda_core_input.lane_center_offset                  = p_vehicle_data->lane_center_offset;
      lcda_core_input.p_pa_data                           = &data;
      lcda_core_input.warn_settings.cvw_rel_vel_range.min = lcda_cals.k_cvw_min_object_curvi_relative_speed[0];
      lcda_core_input.warn_settings.cvw_rel_vel_range.max = lcda_cals.k_cvw_max_object_curvi_relative_speed;

      fbk_output.p_pa_data               = &data;
      fbk_output.p_index_id_lookup_table = &lookup_table;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }
};

#endif // LCDA_PROCESS_CVW_TEST_HPP
