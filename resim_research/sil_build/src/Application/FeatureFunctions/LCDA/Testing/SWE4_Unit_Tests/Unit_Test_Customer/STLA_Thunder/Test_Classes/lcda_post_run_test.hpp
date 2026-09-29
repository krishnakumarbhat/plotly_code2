#ifndef LCDA_POST_RUN_TEST_HPP
#define LCDA_POST_RUN_TEST_HPP

/**
 * @file lcda_post_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class Lcda_Post_Run_Test
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_index_lookup.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_core_calibration.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_input_generator.h"
#include "lcda_input_t.h"
#include "lcda_instance.h"
#include "lcda_output_t.h"
#include "lcda_types.h"
}

class Lcda_Post_Run_Test : public ::testing::Test
{
 protected:
   Lcda_Instance_T lcda_instance{};
   Lcda_Input_T lcda_input{};
   Lcda_Output_T lcda_output{};
   Lcda_Core_Input_T &lcda_core_input   = lcda_instance.core_input;
   Lcda_Core_Output_T &lcda_core_output = lcda_instance.core_output;
   Lcda_Core_Calibration_T &cals        = lcda_instance.calibration;
   Pa_Data_T data{};
   Fbk_Index_Id_Lookup_Table_T fbk_index_id_lookup_table;
   Fbk_Object_Data_T *object_data     = data.object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data = &data.vehicle_data;
   Fbk_Output_T fbk_output;

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp()
   {
      /* Update default cals and set calibration pointer */
      Lcda_Core_Cal_Update_Defaults(&cals);
      /* Initialize context data */
      p_vehicle_data->host_width  = 2.f;
      p_vehicle_data->host_length = 5.f;

      lcda_core_input.p_pa_data = &data;

      lcda_core_output.lcda_status                      = LCDA_STATUS_ACTIVE;
      lcda_core_output.bsw_core_output.f_bsw_is_enabled = FBK_TRUE;
      lcda_core_output.cvw_core_output.f_cvw_is_enabled = FBK_TRUE;
      lcda_core_output.slc_core_output.f_slc_is_enabled = FBK_TRUE;
      lcda_core_output.elc_core_output.f_elc_is_enabled = FBK_TRUE;

      Vector_2d_T zone_p0; // Point zero for zone creation
      zone_p0.x = cals.k_bsw_zone_front_ego_side_x + cals.k_bsw_zone_front_ego_side_x_hys;
      zone_p0.y = cals.k_bsw_zone_rear_outer_side_y + cals.k_bsw_zone_rear_outer_side_y_hys;
      Lcda_Create_Zone(zone_p0, -(cals.k_bsw_zone_rear_outer_side_x - cals.k_bsw_zone_rear_outer_side_x_hys),
                       (cals.k_bsw_zone_rear_outer_side_y + cals.k_bsw_zone_rear_outer_side_y_hys
                        - cals.k_bsw_zone_front_ego_side_y - cals.k_bsw_zone_front_ego_side_y_hys),
                       &lcda_core_input.initial_bsw_zone_hys);
      fbk_output.p_index_id_lookup_table = &fbk_index_id_lookup_table;
   }


   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }
};

#endif /* LCDA_POST_RUN_TEST_HPP */
