#ifndef CTA_CONFLICT_ZONE_ADAPTER_TEST
#define CTA_CONFLICT_ZONE_ADAPTER_TEST

/**
 * @file cta_conflict_zone_adapter_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for cta_conflict_zone_adapter module
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "cta_core_calibration.h"
#include "cta_core_input_t.h"
#include "cta_types.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "pa_context.h"
#include "pa_reuse.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Cta_Conflict_Zone_Adapter_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Cta_Crit_Level_Calibration_T crit_level_cals{};
   Cta_Core_Input_T cta_core_input{};
   Cta_Inters_Zone_Ext_Param_T confl_zone_ext_params{};
   Cta_Object_Data_T object{};
   Cta_Core_Calibration_T core_cals;
   Fbk_Object_Data_T tracker_output{};
   Cta_Object_Attributes_T attributes{};
   Cta_Object_Persistent_T persistent{};

   Pa_Data_T data{};
   Fbk_Output_T fbk_output{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Pa_Context_T context{};

   Cta_Mode_T mode;
   float32_T width;
   float32_T min_cals_val;
   float32_T max_cals_val;
   float32_T init_level_cals_min_array[CTA_NUM_CRIT_LEVEL];
   float32_T init_level_cals_max_array[CTA_NUM_CRIT_LEVEL];


   void SetUp() override
   {
      Cta_Core_Cal_Update_Defaults(&core_cals);

      mode                = CTA_MODE_REAR;
      object.attributes   = &attributes;
      object.tracker_data = tracker_output;
      object.persistent   = &persistent;

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

   void Cta_Init_Conflict_Zone_Borders(Cta_Crit_Level_Calibration_T *p_crit_level_cals,
                                       float32_T *p_init_min_array,
                                       float32_T *p_init_max_array,
                                       Cta_Mode_T init_mode);

 protected:
};

inline void Cta_Conflict_Zone_Adapter_Test::Cta_Init_Conflict_Zone_Borders(Cta_Crit_Level_Calibration_T *p_crit_level_cals,
                                                                           float32_T *p_init_min_array,
                                                                           float32_T *p_init_max_array,
                                                                           Cta_Mode_T init_mode)
{
   for (uint8_t i = 0u; i < CTA_NUM_CRIT_LEVEL; i++)
   {
      p_crit_level_cals->min_long_point_criticality_level[init_mode][i] = p_init_min_array[i];
      p_crit_level_cals->max_long_point_criticality_level[init_mode][i] = p_init_max_array[i];
   }
}

#endif /* CTA_CONFLICT_ZONE_ADAPTER_TEST */
