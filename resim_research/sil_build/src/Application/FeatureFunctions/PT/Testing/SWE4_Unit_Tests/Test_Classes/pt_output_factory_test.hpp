#ifndef PT_OUTPUT_FACTORY_TEST_HPP
#define PT_OUTPUT_FACTORY_TEST_HPP

/**
 * @file pt_output_factory_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for PT output factory functions unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pt_constants.h"
#include "pt_core_calibration.h"
#include "pt_input_t.h"
#include "pt_output_t.h"
#include "pt_persistent_t.h"
#include "pt_types.h"
}

class Pt_Output_Factory_Test : public ::testing::Test
{
 public:
   Pt_Path_T path{};
   Pt_Output_T path_out{};
   Pt_Core_Calibration_T cals;
   Pt_Input_T pt_input{};

   Fbk_Output_T fbk_output{};

   float32_T *p_grid_array = nullptr;

   Pt_Object_T object{};
   Pa_Data_T data{};
   Fbk_Vehicle_Data_T *p_vehicle_data = nullptr;
   /**
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      p_grid_array = pt_input.grid_pt_array;
      Pt_Update_Grid_Array_Defaults(pt_input.grid_pt_array);
      Pt_Core_Cal_Update_Defaults(&cals);

      fbk_output.p_pa_data = &data;
      p_vehicle_data       = &(data.vehicle_data);

      pt_input.p_fbk_output = &fbk_output;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

   void Pt_Init_Path_Points_With_Constants(Pt_Path_T *p_path, float32_T val);
   void Pt_Init_Path_Points_With_Constants_In_A_Defined_Range(Pt_Path_T *p_path, uint8_t start_range, uint8_t end_range, float32_T val);

 protected:
};

inline void Pt_Output_Factory_Test::Pt_Init_Path_Points_With_Constants(Pt_Path_T *p_path, float32_T val)
{
   for (uint8_t i = PT_LOWEST_GRID_POINT_INDEX; i <= PT_HIGHEST_GRID_POINT_INDEX; i++)
   {
      p_path->path_points[i] = val;
   }
}

inline void Pt_Output_Factory_Test::Pt_Init_Path_Points_With_Constants_In_A_Defined_Range(Pt_Path_T *p_path,
                                                                                          uint8_t start_range,
                                                                                          uint8_t end_range,
                                                                                          float32_T val)
{
   for (uint8_t i = start_range; i <= end_range; i++)
   {
      p_path->path_points[i] = val;
   }
}


#endif /* PT_OUTPUT_FACTORY_TEST_HPP */
