#ifndef CTA_TEST
#define CTA_TEST

/**
 * @file cta_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for cta main module
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
#include "cta_instance.h"
#include "cta_persistent_t.h"
#include "cta_types.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_ref_point.h"
#include "fbk_vehicle_data_t.h"
#include "ml_vector_2d_t.h"
#include "pa_context.h"
#include "pa_reuse.h"
#include "pt_output_t.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Cta_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Cta_Instance_T cta_instance{};
   Cta_Object_Data_T object{};
   Cta_Comparison_Data_T cta_comparison_data{};
   Cta_Core_Calibration_T &cals   = cta_instance.calibration;
   Cta_Core_Calibration_T *p_cals = &cals;
   Fbk_Object_Corners_T target_corners{};
   Cta_Crit_Level_Calibration_T crit_level_calibration{};
   Cta_Mode_T mode;

   Fbk_Object_Data_T obj_tracker_output{};
   Cta_Object_Attributes_T attributes{};
   Cta_Object_Persistent_T obj_persistent{};

   Cta_Core_Input_T &core_input                    = cta_instance.core_input;
   Cta_Core_Output_T &core_output                  = cta_instance.core_output;
   Cta_Persistent_T &cta_persistent                = cta_instance.persistent;
   Cta_Object_Persistent_T *p_obj_persistent_array = cta_instance.obj_persistent_array;

   Pa_Data_T data{};
   Fbk_Output_T fbk_output{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;

   Pt_Path_Object_Pair_Output_T pt_match_output{};

   void SetUp() override
   {
      object.attributes                  = &attributes;
      object.attributes->p_pt_match_info = &pt_match_output;
      object.tracker_data                = obj_tracker_output;
      object.persistent                  = &obj_persistent;

      Cta_Core_Cal_Update_Defaults(p_cals);

      fbk_output.p_pa_data = &data;
      object_data          = data.object_data;
      p_vehicle_data       = &(data.vehicle_data);

      p_vehicle_data->host_length = 5.0f;
      p_vehicle_data->host_width  = 2.0f;

      cals.k_cta_intersection_line_host_width_percentage = 0.0f;

      core_input.p_pa_data = &(data);
      mode                 = CTA_MODE_REAR;

      Cta_Init_Zone(&core_input.cta_zone, p_cals);
      Cta_Init_Wrapping_Parameters(core_input.ttc_criticality_level, p_cals);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

   void Cta_Init_Zone(Fbk_Field_Of_Interest_T *p_cta_zone, Cta_Core_Calibration_T *p_cta_cals);
   void Cta_Init_Wrapping_Parameters(float32_T ttc_criticality_level[CTA_NUM_MODES][CTA_NUM_CRIT_LEVEL],
                                     Cta_Core_Calibration_T *p_cta_cals);

 protected:
};

void Cta_Test::Cta_Init_Zone(Fbk_Field_Of_Interest_T *p_cta_zone, Cta_Core_Calibration_T *p_cta_cals)
{
   uint8_t i;
   p_cta_zone->size = p_cta_cals->k_cta_amount_butterfly_points_in_use;
   for (i = 0; i < p_cta_zone->size; i++)
   {
      p_cta_zone->points[i].x = p_cta_cals->k_cta_butterfly_long[i];
      p_cta_zone->points[i].y = p_cta_cals->k_cta_butterfly_lat[i];
   }
}


void Cta_Test::Cta_Init_Wrapping_Parameters(float32_T ttc_criticality_level[CTA_NUM_MODES][CTA_NUM_CRIT_LEVEL],
                                            Cta_Core_Calibration_T *p_cta_cals)
{
   uint8_t mode_idx;
   uint8_t level_idx;
   for (mode_idx = FBK_ZERO_UINT; mode_idx < CTA_NUM_MODES; mode_idx++)
   {
      for (level_idx = FBK_ZERO_UINT; level_idx < CTA_NUM_CRIT_LEVEL; level_idx++)
      {
         ttc_criticality_level[mode_idx][level_idx] = p_cta_cals->k_cta_ttc_criticality_level[mode_idx][level_idx];
      }
   }
}


#endif /* CTA_TEST */
