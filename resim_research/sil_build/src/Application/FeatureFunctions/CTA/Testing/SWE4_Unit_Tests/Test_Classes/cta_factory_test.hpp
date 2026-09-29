#ifndef CTA_FACTORY_TEST
#define CTA_FACTORY_TEST

/**
 * @file cta_factory_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for cta_factory module
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "cta_core_calibration.h"
#include "cta_core_input_t.h"
#include "cta_instance.h"
#include "cta_types.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "pa_const_macros.h"
#include "pa_context.h"
#include "pt_output_t.h"
}

class Cta_Factory_Test : public ::testing::Test
{
 public:
   /**
    * Here is the place for some initializations of public members
    */
   Cta_Instance_T cta_instance{};
   Cta_Core_Calibration_T *p_cals   = nullptr;
   Cta_Core_Input_T &cta_core_input = cta_instance.core_input;
   Fbk_Object_Data_T tracker_output{};
   Cta_Object_Attributes_T attributes{};
   Cta_Object_Persistent_T persistent{};
   Cta_Object_Data_T object{};
   Cta_Comparison_Data_T cta_comparison_data{};

   Cta_Inters_Zone_Ext_Param_T zone_params{};

   Pt_Output_T pt_output{};
   Pt_Output_T *p_pt_output;
   Pt_Path_Object_Pair_Output_T pt_match_info{};
   Pt_Nearest_Path_T pt_nearest_path_info{};

   Pa_Data_T data{};
   Fbk_Output_T fbk_output{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;

   void SetUp() override
   {
      p_cals = &cta_instance.calibration;
      Cta_Core_Cal_Update_Defaults(p_cals);

      object.attributes   = &attributes;
      object.tracker_data = tracker_output;
      object.persistent   = &persistent;


      pt_output.nearest_path_output[0]    = pt_nearest_path_info;
      pt_output.path_obj_pair_output[0]   = pt_match_info;
      p_pt_output                         = &pt_output;
      cta_core_input.p_pa_data            = &data;
      cta_core_input.p_pt_output          = p_pt_output;
      cta_instance.core_input.p_pa_data   = &data;
      cta_instance.core_input.p_pt_output = p_pt_output;

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

 protected:
   /**
    * Protected Members
    */
};

#endif // CTA_FACTORY_TEST