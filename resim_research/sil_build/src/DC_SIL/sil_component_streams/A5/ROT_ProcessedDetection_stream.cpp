#include <stdint.h> //compiler warning fixes , std definition overlapping with fixmac.h file
#include "rot_processed_detection_stream.h"
#include "radar_ecu_CORE1.h"
#include "f360_log_types.h"
#include "rspp_radar_sensor.h"
#include "f360_host_calib.h"
#include "dc_config.h"

DC_ROT_Processed_Detection_Stream_T *GetROTProcessedDetDataPtr();
DC_ROT_Processed_Detection_Stream_T gen7_rot_processed_dets;

void CopyTrackerOutToROTDetStream(rspp_variant_A::F360_Radar_Sensor_T *sens, f360_variant_A::F360_Detection_Log_Output_T *log_dets, f360_variant_A::F360_Host_Calib_T *host_calibs, Run_Mode_T run_mode) {

   DC_ROT_Processed_Detection_Stream_T *processed_dets = GetROTProcessedDetDataPtr();
   memset(processed_dets, 0, sizeof(DC_ROT_Processed_Detection_Stream_T));

   for (int i = 0; i < ROT_NUM_TOTAL_RADAR_SENSORS; i++) {
      processed_dets->sensorInfoLog->f_sens_valid = sens->variable.is_valid;
      // processed_dets->sensorInfoLog-> =
      processed_dets->sensorInfoLog->look_id        = sens->variable.look_id;
      processed_dets->sensorInfoLog->look_index     = sens->variable.look_index;
      processed_dets->sensorInfoLog->mount_location = sens->constant.mounting_location;
      // processed_dets->sensorInfoLog->new_measurement_update =
      processed_dets->sensorInfoLog->vcs_lat_posn   = sens->constant.mounting_position.vcs_position.lateral;
      processed_dets->sensorInfoLog->vcs_long_posn  = sens->constant.mounting_position.vcs_position.longitudinal;
      processed_dets->sensorInfoLog->vcs_lat_vel    = sens->variable.vcs_velocity.lateral;
      processed_dets->sensorInfoLog->vcs_long_vel   = sens->variable.vcs_velocity.longitudinal;
      processed_dets->sensorInfoLog->iso_lat_posn   = -sens->constant.mounting_position.vcs_position.lateral;
      processed_dets->sensorInfoLog->iso_long_posn  = sens->constant.mounting_position.vcs_position.longitudinal;
      processed_dets->sensorInfoLog->iso_lat_vel    = -sens->variable.vcs_velocity.lateral;
      processed_dets->sensorInfoLog->iso_long_vel   = sens->variable.vcs_velocity.longitudinal;
      processed_dets->sensorInfoLog->radar_polarity = sens->constant.polarity;
      // processed_dets->sensorInfoLog->range_rate_interval_width =
      processed_dets->sensorInfoLog->sensor_type              = sens->constant.sensor_type;
      processed_dets->sensorInfoLog->sensorID                 = sens->constant.id;
      processed_dets->sensorInfoLog->vcs_boresight_az_angle   = sens->constant.mounting_position.vcs_boresight_azimuth_angle;
      processed_dets->sensorInfoLog->vcs_boresight_elev_angle = sens->constant.mounting_position.vcs_boresight_elevation_angle;
      processed_dets->sensorInfoLog->iso_boresight_az_angle   = sens->constant.mounting_position.vcs_boresight_azimuth_angle;
      processed_dets->sensorInfoLog->iso_boresight_elev_angle = sens->constant.mounting_position.vcs_boresight_elevation_angle;
   }
   for (size_t j = 0; j < DC_ROT_MAX_NUMBER_OF_DETECTIONS && j < log_dets->f360header.num_elements; j++) {

      processed_dets->detection_info.detection[j].vcs_y             = log_dets->detection[j].vcs_y;
      processed_dets->detection_info.detection[j].iso_y             = -log_dets->detection[j].vcs_y;
      processed_dets->detection_info.detection[j].rngrate_dealiased = log_dets->detection[j].rngrate_dealiased;
      processed_dets->detection_info.detection[j].rngrate_comp      = log_dets->detection[j].rngrate_comp;
      processed_dets->detection_info.detection[j].raw_det_id        = log_dets->detection[j].raw_det_id;
      processed_dets->detection_info.detection[j].vcs_x             = log_dets->detection[j].vcs_x;
      if (run_mode == DETECTIONS_UDP) {
         processed_dets->detection_info.detection[j].iso_x = log_dets->detection[j].vcs_x + host_calibs->dist_rear_axle_to_vcs_m;
      } else {
         processed_dets->detection_info.detection[j].iso_x = log_dets->detection[j].vcs_x;
      }
      processed_dets->detection_info.detection[j].objTrkID                 = log_dets->detection[j].objTrkID;
      processed_dets->detection_info.detection[j].clusterID                = log_dets->detection[j].clusterID;
      processed_dets->detection_info.detection[j].sensorID                 = log_dets->detection[j].sensorID;
      processed_dets->detection_info.detection[j].motion_status            = log_dets->detection[j].motion_status;
      processed_dets->detection_info.detection[j].wheel_spin               = log_dets->detection[j].wheel_spin;
      processed_dets->detection_info.detection[j].f_dealiased              = log_dets->detection[j].f_dealiased;
      processed_dets->detection_info.detection[j].f_double_bounce          = log_dets->detection[j].f_double_bounce;
      processed_dets->detection_info.detection[j].f_FOV_edge               = log_dets->detection[j].f_FOV_edge;
      processed_dets->detection_info.detection[j].f_close_target           = log_dets->detection[j].f_close_target;
      processed_dets->detection_info.detection[j].f_ok_to_use              = log_dets->detection[j].f_ok_to_use;
      processed_dets->detection_info.detection[j].f_inside_gate            = log_dets->detection[j].f_inside_gate;
      processed_dets->detection_info.detection[j].f_rr_inlier              = log_dets->detection[j].f_rr_inlier;
      processed_dets->detection_info.detection[j].f_used_in_rr_msmt_update = log_dets->detection[j].f_used_in_rr_msmt_update;
      processed_dets->detection_info.detection[j].f_on_guardrail           = log_dets->detection[j].f_on_guardrail;
   }
}

DC_ROT_Processed_Detection_Stream_T *GetROTProcessedDetDataPtr() {
   return &gen7_rot_processed_dets;
}