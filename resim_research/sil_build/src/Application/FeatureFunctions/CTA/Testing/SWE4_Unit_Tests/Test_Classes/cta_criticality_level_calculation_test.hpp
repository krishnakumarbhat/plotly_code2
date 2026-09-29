#ifndef CTA_CRITICALITY_LEVEL_CALCULATION_TEST
#define CTA_CRITICALITY_LEVEL_CALCULATION_TEST

/**
 * @file cta_criticality_level_calculation_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for cta_criticality_level_calculation module
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "cta_core_calibration.h"
#include "cta_core_input_t.h"
#include "cta_core_output_t.h"
#include "cta_persistent_t.h"
#include "cta_types.h"
#include "fbk_field_of_interest.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_ref_point.h"
#include "fbk_vehicle_data_t.h"
#include "ml_vector_2d_t.h"
#include "pa_context.h"
#include "pa_reuse.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Cta_Criticality_Level_Calculation_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Cta_Crit_Level_Calibration_T crit_level_cals{};
   Cta_Object_Data_T object{};
   Cta_Comparison_Data_T cta_comparison_data{};
   Cta_Core_Calibration_T cals;
   Fbk_Object_Corners_T target_corners{};

   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Fbk_Output_T fbk_output{};

   Fbk_Object_Data_T obj_tracker_output{};
   Cta_Object_Attributes_T attributes{};
   Cta_Object_Persistent_T obj_persistent{};

   Cta_Core_Input_T core_input{};
   Cta_Core_Output_T core_output{};

   void SetUp() override
   {
      object.attributes   = &attributes;
      object.tracker_data = obj_tracker_output;
      object.persistent   = &obj_persistent;
      Cta_Core_Cal_Update_Defaults(&cals);

      fbk_output.p_pa_data = &data;
      object_data          = data.object_data;
      p_vehicle_data       = &(data.vehicle_data);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

   void Cta_Init_Level_Calibration_One_Level(Cta_Crit_Level_Calibration_T *p_level_cals_extended,
                                             uint8_t level_index,
                                             Cta_Mode_T mode,
                                             float32_T long_max_level,
                                             float32_T long_min_level,
                                             float32_T ttc_level);

 protected:
};

void Cta_Criticality_Level_Calculation_Test::Cta_Init_Level_Calibration_One_Level(Cta_Crit_Level_Calibration_T *p_level_cals_extended,
                                                                                  uint8_t level_index,
                                                                                  Cta_Mode_T mode,
                                                                                  float32_T long_max_level,
                                                                                  float32_T long_min_level,
                                                                                  float32_T ttc_level)
{
   p_level_cals_extended->max_long_point_criticality_level[mode][level_index] = long_max_level;
   p_level_cals_extended->min_long_point_criticality_level[mode][level_index] = long_min_level;
   p_level_cals_extended->ttc_criticality_level[mode][level_index]            = ttc_level;
}

#endif /* CTA_CRITICALITY_LEVEL_CALCULATION_TEST */
