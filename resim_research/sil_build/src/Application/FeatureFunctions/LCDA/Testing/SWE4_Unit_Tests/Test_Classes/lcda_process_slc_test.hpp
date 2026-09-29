#ifndef LCDA_PROCESS_SLC_TEST_HPP
#define LCDA_PROCESS_SLC_TEST_HPP

/**
 * @file lcda_process_slc_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class Lcda_Process_Slc_Test
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "lcda.h"
#include "lcda_core_calibration.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_instance.h"
#include "lcda_persistent_t.h"
#include "lcda_process_slc.h" // IWYU pragma: keep
#include "lcda_types.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}


/**
 * Class used to create a fixture for LCDA_ProcessSimLaneChange_test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Lcda_Process_Slc_Test : public ::testing::Test
{
 protected:
   Lcda_Instance_T lcda_instance{};
   Pa_Data_T data{};
   Fbk_Index_Id_Lookup_Table_T lookup_table{};

   Lcda_Core_Calibration_T &lcda_cals      = lcda_instance.calibration;
   Fbk_Object_Data_T *object_data          = data.object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data      = &data.vehicle_data;
   Lcda_Core_Input_T &lcda_core_input      = lcda_instance.core_input;
   Lcda_Slc_Core_Output_T &slc_core_output = lcda_instance.core_output.slc_core_output;
   // Lcda_Persistent_T &lcda_persistent      = lcda_instance.persistent;
   Lcda_Slc_Persistent_T &slc_persistent = lcda_instance.slc_persistent;

   Slc_Object_T slc_object{};
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

      Lcda_Reset(&lcda_instance, &data);

      slc_object.p_tracker_data = &tracker_object;

      lcda_cals.k_lcda_min_lane_width = 2.0f;
      lcda_cals.k_lcda_max_lane_width = 7.0f;

      lcda_cals.k_zone_hys_obj_width_correction = 1.6f;
      lcda_cals.k_cvw_zone_y_hys_max            = 5.0f;

      lcda_cals.k_slc_zone_x[0] = -5.0f;
      lcda_cals.k_slc_zone_x[1] = -40.0f;
      lcda_cals.k_slc_zone_x[2] = -90.0f;
      lcda_cals.k_slc_zone_x[3] = -90.0f;
      lcda_cals.k_slc_zone_x[4] = -40.0f;
      lcda_cals.k_slc_zone_x[5] = -5.0f;

      lcda_cals.k_slc_zone_y[0] = 3.0f;
      lcda_cals.k_slc_zone_y[1] = 3.0f;
      lcda_cals.k_slc_zone_y[2] = 3.0f;
      lcda_cals.k_slc_zone_y[3] = 1.0f;
      lcda_cals.k_slc_zone_y[4] = 1.0f;
      lcda_cals.k_slc_zone_y[5] = 1.0f;

      lcda_cals.k_slc_zone_y_hys[0] = 0.1f;
      lcda_cals.k_slc_zone_y_hys[1] = 0.1f;
      lcda_cals.k_slc_zone_y_hys[2] = 0.1f;
      lcda_cals.k_slc_zone_y_hys[3] = 0.1f;
      lcda_cals.k_slc_zone_y_hys[4] = 0.1f;
      lcda_cals.k_slc_zone_y_hys[5] = 0.1f;

      lcda_cals.k_slc_max_curvi_heading_abs      = 0.785f;
      lcda_cals.k_slc_min_obj_curvi_long_vel_abs = 1.0f;
      lcda_cals.k_slc_max_obj_eclipse            = 0.5f;

      lcda_cals.k_lcda_min_exist_prop = 0.7f;
      lcda_cals.k_lcda_min_track_age  = 2;

      lcda_cals.k_slc_lateral_ttc_lookup[0] = 0.0f;
      lcda_cals.k_slc_lateral_ttc_lookup[1] = 1.0f;
      lcda_cals.k_slc_lateral_ttc_lookup[2] = 2.0f;
      lcda_cals.k_slc_lateral_ttc_lookup[3] = 3.0f;
      lcda_cals.k_slc_lateral_ttc_lookup[4] = 4.0f;
      lcda_cals.k_slc_lateral_ttc_lookup[5] = 5.0f;

      lcda_cals.k_slc_lane_change_prob_lookup[0] = 1.0f;
      lcda_cals.k_slc_lane_change_prob_lookup[1] = 0.9f;
      lcda_cals.k_slc_lane_change_prob_lookup[2] = 0.8f;
      lcda_cals.k_slc_lane_change_prob_lookup[3] = 0.7f;
      lcda_cals.k_slc_lane_change_prob_lookup[4] = 0.6f;
      lcda_cals.k_slc_lane_change_prob_lookup[5] = 0.6f;

      lcda_cals.k_slc_warntrigger_TTC_lon_late  = -1.05f;
      lcda_cals.k_slc_warntrigger_TTC_lat_late  = -0.9f;
      lcda_cals.k_slc_warntrigger_TTC_lon_early = 0.7f;
      lcda_cals.k_slc_warntrigger_TTC_lat_early = 0.6f;

      lcda_cals.k_slc_critical_lat_ttc = 3.0f;
      lcda_cals.k_slc_critical_lon_ttc = 3.5f;

      lcda_cals.k_slc_critical_lat_ttc_hys = 1.0f;
      lcda_cals.k_slc_critical_lon_ttc_hys = 1.5f;

      lcda_cals.k_slc_min_mature_cycles        = 1;
      lcda_cals.k_slc_alert_qualifying_counter = 3;

      lcda_cals.k_lcda_zone_intersect_critical_point_lateral_ratio = 0.0f;


      /* Set basic vehicle data */
      p_vehicle_data->host_length        = 4.5f;
      p_vehicle_data->host_width         = 2.0f;
      p_vehicle_data->lane_width         = 3.0f;
      p_vehicle_data->lane_center_offset = 0.0f;

      // Set lane width from camera info
      lcda_core_input.lane_width         = 2.0f;
      lcda_core_input.lane_center_offset = 0.0f;

      // Initialize Sim Lane Change core
      Lcda_Reset_Slc_Core(&slc_core_output, &slc_persistent);

      lcda_core_input.warn_settings.slc_ttc_thres_lat = lcda_cals.k_slc_critical_lat_ttc;
      lcda_core_input.warn_settings.slc_ttc_thres_lon = lcda_cals.k_slc_critical_lon_ttc;

      lcda_core_input.p_pa_data = &data;

      fbk_output.p_index_id_lookup_table = &lookup_table;
      fbk_output.p_pa_data               = &data;
   }


   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 public:
   void Lcda_Create_Valid_Slc_Track(Fbk_Object_Data_T *p_tracker_object,
                                    uint8_t obj_idx,
                                    float32_T lat_pos,
                                    float32_T lon_pos,
                                    float32_T lat_vel_rel,
                                    float32_T long_vel_rel);
};

/* required helper functions to create tracks or slc_objects */
void Lcda_Process_Slc_Test::Lcda_Create_Valid_Slc_Track(Fbk_Object_Data_T *p_tracker_object,
                                                        uint8_t obj_idx,
                                                        float32_T lat_pos,
                                                        float32_T lon_pos,
                                                        float32_T lat_vel_rel,
                                                        float32_T long_vel_rel)
{
   p_tracker_object->index                 = obj_idx;
   p_tracker_object->status                = PA_OBJ_STATUS_MATURE;
   p_tracker_object->curvi_heading         = 0.2f;
   p_tracker_object->curvi_vel.x           = 20.0f;
   p_tracker_object->age                   = 8;
   p_tracker_object->vcs_pos.x             = lon_pos;
   p_tracker_object->vcs_pos.y             = lat_pos;
   p_tracker_object->width                 = 2.0f;
   p_tracker_object->length                = 6.0f;
   p_tracker_object->curvi_vel_rel.y       = lat_vel_rel;
   p_tracker_object->curvi_vel_rel.x       = long_vel_rel;
   p_tracker_object->vcs_vel_rel.x         = long_vel_rel;
   p_tracker_object->curvi_pos.y           = lat_pos;
   p_tracker_object->curvi_pos.x           = lon_pos;
   p_tracker_object->id                    = obj_idx + 1;
   p_tracker_object->unique_id             = obj_idx + 1;
   p_tracker_object->eclipse_value         = 0.0f;
   p_tracker_object->existence_probability = 1.0f;
}

#endif /* LCDA_PROCESS_SLC_TEST_HPP */
