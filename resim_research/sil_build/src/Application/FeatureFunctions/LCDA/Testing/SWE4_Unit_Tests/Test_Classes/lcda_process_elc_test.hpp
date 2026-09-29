#ifndef LCDA_PROCESS_ELC_TEST_HPP
#define LCDA_PROCESS_ELC_TEST_HPP

/**
 * @file lcda_process_elc_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class Lcda_Process_Elc_Test
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_core_calibration.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_instance.h"
#include "lcda_persistent_t.h"
#include "lcda_process_elc.h" // IWYU pragma: keep
#include "lcda_types.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}

/**
 * Class used to create a fixture for LCDA_ProcessElc_test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Lcda_Process_Elc_Test : public ::testing::Test
{
 protected:
   Lcda_Instance_T lcda_instance{};
   Pa_Data_T data{};
   Fbk_Index_Id_Lookup_Table_T lookup_table{};

   Lcda_Core_Calibration_T &lcda_cals      = lcda_instance.calibration;
   Fbk_Object_Data_T *object_data          = data.object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data      = &data.vehicle_data;
   Lcda_Core_Input_T &lcda_core_input      = lcda_instance.core_input;
   Lcda_Elc_Core_Output_T &elc_core_output = lcda_instance.core_output.elc_core_output;
   // Lcda_Persistent_T &lcda_persistent      = lcda_instance.persistent;
   Lcda_Elc_Persistent_T &elc_persistent = lcda_instance.elc_persistent;

   Elc_Object_T elc_object{};
   Fbk_Object_Data_T tracker_object{};
   Fbk_Output_T fbk_output{};
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      /* Update default cals and set calibration pointer */
      Lcda_Core_Cal_Update_Defaults(&lcda_cals);

      /* Initialize core input */
      lcda_core_input.p_pa_data = &data;

      elc_object.p_tracker_data = &tracker_object;

      /* Set basic vehicle data */
      p_vehicle_data->host_length        = 4.5f;
      p_vehicle_data->host_width         = 2.0f;
      p_vehicle_data->lane_width         = 3.0f;
      p_vehicle_data->lane_center_offset = 0.0f;

      // Set lane width from camera info
      lcda_core_input.lane_width         = 2.0f;
      lcda_core_input.lane_center_offset = 0.0f;

      // Initialize Sim Lane Change core
      Lcda_Reset_Elc_Core(&elc_core_output, &elc_persistent);
      fbk_output.p_pa_data               = &data;
      fbk_output.p_index_id_lookup_table = &lookup_table;
   }


   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 public:
   void Lcda_Create_Valid_Elc_Track(Fbk_Object_Data_T *p_tracker_object,
                                    uint8_t obj_idx,
                                    float32_T lat_pos,
                                    float32_T lon_pos,
                                    float32_T lat_vel_rel,
                                    float32_T long_vel_rel);
};

/* required helper functions to create tracks or slc_objects */
void Lcda_Process_Elc_Test::Lcda_Create_Valid_Elc_Track(Fbk_Object_Data_T *p_tracker_object,
                                                        uint8_t obj_id,
                                                        float32_T lat_pos,
                                                        float32_T lon_pos,
                                                        float32_T lat_vel_rel,
                                                        float32_T long_vel_rel)
{
   p_tracker_object->index                 = obj_id - 1u;
   p_tracker_object->status                = PA_OBJ_STATUS_MATURE;
   p_tracker_object->curvi_heading         = 0.2f;
   p_tracker_object->curvi_vel.x           = 20.0f;
   p_tracker_object->age                   = lcda_cals.k_lcda_min_track_age;
   p_tracker_object->vcs_pos.x             = lon_pos;
   p_tracker_object->vcs_pos.y             = lat_pos;
   p_tracker_object->width                 = 2.0f;
   p_tracker_object->length                = 5.0f;
   p_tracker_object->curvi_vel_rel.x       = long_vel_rel;
   p_tracker_object->curvi_vel_rel.y       = lat_vel_rel;
   p_tracker_object->curvi_pos.x           = lon_pos;
   p_tracker_object->curvi_pos.y           = lat_pos;
   p_tracker_object->id                    = obj_id;
   p_tracker_object->existence_probability = 1.0f;
}

#endif /* LCDA_PROCESS_ELC_TEST_HPP */
