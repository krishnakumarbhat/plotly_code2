#ifndef LTB_TIME_CALCULATION_HPP
#define LTB_TIME_CALCULATION_HPP

/**
 * @file ltb_intersection_analyzer_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for LTB unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include <gtest/gtest.h> // IWYU pragma: keep
#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "fbk_circular_shape_calculator.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "ltb_core_calibration.h"
#include "ltb_core_input_t.h"
#include "ltb_core_output_t.h"
#include "ltb_factory.h"
#include "ltb_time_calculation.h"
#include "ltb_types.h"
}

class Ltb_Time_Calculation_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Ltb_Core_Calibration_T ltb_cals;
   Fbk_Waypoint_with_Circle_Centers_T ego{};
   Fbk_Waypoint_with_Circle_Centers_T obj{};

   Ltb_Object_T ltb_object{};
   Ltb_Core_Input_T ltb_core_input{};
   Ltb_Core_Output_T ltb_core_output{};
   Ltb_Persistent_T ltb_persistent{};
   Ltb_Persistent_T *p_ltb_persistent;
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Fbk_Trajectory_T ego_trajectory{};

   void SetUp() override
   {
      Ltb_Core_Cal_Update_Defaults(&ltb_cals);

      Fbk_Init_Waypoint_with_Circle_Centers_Structure(&ego);
      Fbk_Init_Waypoint_with_Circle_Centers_Structure(&obj);

      /* Initialize context data */
      object_data    = data.object_data;
      p_vehicle_data = &data.vehicle_data;

      p_ltb_persistent = &ltb_persistent;

      Ltb_Core_Cal_Update_Defaults(&ltb_cals);

      Ltb_Init_Prediction_Time_Step(p_ltb_persistent, &ltb_cals);

      Ltb_Reset_Trajectory(&ego_trajectory, &ltb_cals);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }


 protected:
};
#endif /*LTB_TIME_CALCULATION_HPP*/
