/******************************************************************************
* Copyright 2024 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
#include "f360_cvt_runner.h"
#include "f360_get_wall_time.h"

namespace f360_variant_A
{
   void Run_CV_Trailer_Estimator(
      const F360_Calibrations_T& calibrations,
      const F360_Host_T& host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Tracker_Info_T& tracker_info,
      F360_CVT_State_T& cvt_state,
      F360_TRKR_TIMING_INFO_T& timing_info)
   {
      const float32_t start_time = get_wall_time();

      if (!cvt_state.f_init_complete)
      {
         F360_CVT_Initialization_Data_T cvt_init;

         int32_t left_rear_sensor_idx = -1;
         int32_t right_rear_sensor_idx = -1;
         for (int32_t i = 0; i < static_cast<int32_t>(MAX_NUMBER_OF_SRR_SENSORS); i++)
         {
            left_rear_sensor_idx = (sensors[i].constant.mounting_location == F360_MOUNTING_LOCATION_LEFT_REAR) ? i : left_rear_sensor_idx;
            right_rear_sensor_idx = (sensors[i].constant.mounting_location == F360_MOUNTING_LOCATION_RIGHT_REAR) ? i : right_rear_sensor_idx;
         }

         bool f_relevant_radar_found = true;
         if (left_rear_sensor_idx >= 0)
         {
            cvt_init.radar_id = sensors[left_rear_sensor_idx].constant.id;
            cvt_init.radar_vcs_latpos = sensors[left_rear_sensor_idx].constant.mounting_position.vcs_position.lateral;
            cvt_init.radar_vcs_longpos = sensors[left_rear_sensor_idx].constant.mounting_position.vcs_position.longitudinal;
         }
         else if (right_rear_sensor_idx >= 0)
         {
            cvt_init.radar_id = sensors[right_rear_sensor_idx].constant.id;
            cvt_init.radar_vcs_latpos = sensors[right_rear_sensor_idx].constant.mounting_position.vcs_position.lateral;
            cvt_init.radar_vcs_longpos = sensors[right_rear_sensor_idx].constant.mounting_position.vcs_position.longitudinal;
         }
         else
         {
            f_relevant_radar_found = false;
         }

         if (f_relevant_radar_found)
         {
            cvt_init.host_length = host.vehicle_length;
            cvt_init.host_width = host.vehicle_width;
            cvt_init.host_rear_axle_vcs_longpos = -host.dist_rear_axle_to_vcs_m;

            cvt_init.joint1_vcs_longpos_min = -1.0F * host.vehicle_length - 3.5F; // 3.5m behind host
            cvt_init.joint1_vcs_longpos_max = -0.5F * host.vehicle_length + 0.5F; // 0.5m ahead of mid host
            cvt_init.link1_wheelbase_min = 3.0F;
            cvt_init.link1_wheelbase_max = 30.0F;
            cvt_init.link2_wheelbase_min = 5.0F;
            cvt_init.link2_wheelbase_max = 30.0F;

            cvt_init.one_link_model.n_updates = 0;
            cvt_init.one_link_model.link1_angle = 0.0F;
            cvt_init.one_link_model.joint1_vcs_longpos = -host.dist_rear_axle_to_vcs_m;
            cvt_init.one_link_model.link1_wheelbase = 10.0F;
            cvt_init.one_link_model.full_vehicle_length = host.vehicle_length + cvt_init.one_link_model.link1_wheelbase + 2.0F;

            cvt_init.two_link_model.n_updates = 0;
            cvt_init.two_link_model.link1_angle = 0.0F;
            cvt_init.two_link_model.link2_angle = 0.0F;
            cvt_init.two_link_model.joint1_vcs_longpos = -host.vehicle_length;
            cvt_init.two_link_model.link1_wheelbase = 5.0F;
            cvt_init.two_link_model.link2_wheelbase = 10.0F;
            cvt_init.two_link_model.full_vehicle_length = host.vehicle_length + cvt_init.two_link_model.link1_wheelbase + cvt_init.two_link_model.link2_wheelbase + 2.0F;

            CVT_Initialize(calibrations, cvt_init, cvt_state);
         }
      }

      if (cvt_state.f_init_complete)
      {
         static F360_CVT_Input_Data_T cvt_input = {};
         cvt_input.tracker_index = tracker_info.cnt_loops;

         // vehicle dynamics
         cvt_input.host_speed = host.vcs_speed;
         cvt_input.host_yawrate = host.yaw_rate_rad;
         cvt_input.host_rear_axle_vcs_longpos = (-1.0F) * host.dist_rear_axle_to_vcs_m;
         cvt_input.host_side_slip_vcs = host.vcs_sideslip;

         // detections info
         int32_t n_det = 0;
         const uint32_t max_num_dets = std::min(static_cast<uint32_t>(CVT_MAX_NUMBER_OF_DETECTIONS), raw_detect_list.number_of_valid_detections);
         for (uint32_t i = 0U; i < max_num_dets; i++)
         {
            if (raw_detect_list.detections[i].raw.sensor_id == static_cast<int32_t>(cvt_state.radar_id))
            {
               cvt_input.detections[n_det].vcs_latpos = det_props[i].vcs_position.y;
               cvt_input.detections[n_det].vcs_longpos = det_props[i].vcs_position.x;
               cvt_input.detections[n_det].range_rate = raw_detect_list.detections[i].raw.range_rate;
               n_det++;
            }
         }
         cvt_input.n_detections = n_det;

         // Run the trailer estimator
         CVT_Execute(calibrations, cvt_input, cvt_state);
      }

      timing_info.cv_trailer_estimator = get_wall_time() - start_time;
   }
}
