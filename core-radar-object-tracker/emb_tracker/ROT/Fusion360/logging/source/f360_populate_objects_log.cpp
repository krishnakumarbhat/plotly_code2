/*===================================================================================*\
 * FILE:  f360_populate_objects_log.cpp
 *====================================================================================
 * Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose."
 *------------------------------------------------------------------------------------
 * Applicable Standards (in order of precedence: highest first):
 *     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
 *     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
 */

#include "f360_populate_objects_log.h"
#include "f360_calculate_priority.h"
#include "f360_norm_heading_angle.h"
#include "f360_radar_sensor.h"
#include "f360_reference_point_support_functions.h"
#include "f360_shrinking_supporting_functions.h"
#include "f360_trk_fltr_cca_states.h"
#include "f360_trk_fltr_ctca_states.h"
#include <algorithm>
#include <cstring>

namespace f360_variant_A
{
static float32_t Compute_Heading_Rate_Variance(const float32_t heading_rate);
static uint16_t Compute_Object_Sensor_Source(const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
                                             const F360_Object_Track_T &object_track, const F360_Detection_List_T &det_list);

void Populate_Objects_Data(F360_Object_Track_T (&objects)[NUMBER_OF_OBJECT_TRACKS], F360_Tracker_Info_T &tracker_info,
                           const F360_Object_Log_T (&objects_log)[NUMBER_OF_OBJECT_TRACKS], const F360_Host_T &host,
                           const F360_Calibrations_T &calibrations)
{
   // Clear id lists
   (void)memset(&tracker_info.active_obj_ids[0], 0, sizeof(tracker_info.active_obj_ids));
   (void)memset(&tracker_info.inactive_obj_ids[0], 0, sizeof(tracker_info.inactive_obj_ids));
   (void)memset(&tracker_info.reduced_active_obj_ids[0], 0, sizeof(tracker_info.reduced_active_obj_ids));
   (void)memset(&tracker_info.reduced_inactive_obj_ids[0], 0, sizeof(tracker_info.reduced_inactive_obj_ids));

   tracker_info.num_active_objs         = 0;
   tracker_info.num_unique_objs         = 0U;
   tracker_info.reduced_num_active_objs = 0;

   uint32_t unique_objs[NUMBER_OF_OBJECT_TRACKS];
   bool reduced_obj_idx_used[NUMBER_OF_REDUCED_OBJECT_TRACKS];
   (void)memset(&unique_objs[0], 0, sizeof(unique_objs));
   (void)memset(&reduced_obj_idx_used[0], 0, sizeof(reduced_obj_idx_used));

   for (uint32_t i = 0U; i < NUMBER_OF_OBJECT_TRACKS; i++)
   {
      const F360_Object_Log_T &log_object_track = objects_log[i];

      if ((0U < log_object_track.status) && (0U < log_object_track.trkID))
      {
         const uint16_t obj_idx            = log_object_track.trkID - 1U;
         F360_Object_Track_T &object_track = objects[obj_idx];

         object_track.unique_id  = log_object_track.unique_id;
         object_track.id         = static_cast<int32_t>(log_object_track.trkID);
         object_track.reduced_id = static_cast<int32_t>(log_object_track.reducedID);

         object_track.vcs_position.x            = log_object_track.xposn;
         object_track.vcs_position.y            = log_object_track.yposn;
         object_track.vcs_velocity.longitudinal = log_object_track.xvel;
         object_track.vcs_velocity.lateral      = log_object_track.yvel;
         object_track.vcs_accel.longitudinal    = log_object_track.xaccel;
         object_track.vcs_accel.lateral         = log_object_track.yaccel;

         object_track.vcs_heading = Angle{log_object_track.heading};
         object_track.curvature   = log_object_track.curvature;
         object_track.speed       = log_object_track.speed;
         object_track.tang_accel  = log_object_track.tang_accel;

         (void)memset(object_track.errcov, 0, sizeof(object_track.errcov));
         if (F360_TRACKER_TRKFLTR_CCV == log_object_track.trk_fltr_type)
         {
            // Mapping CCV to CCA
            object_track.errcov[0][0] = log_object_track.xposn_var;
            object_track.errcov[1][1] = log_object_track.xvel_var;
            object_track.errcov[2][2] = 1.0F;
            object_track.errcov[3][3] = log_object_track.yposn_var;
            object_track.errcov[4][4] = log_object_track.yvel_var;
            object_track.errcov[5][5] = 1.0F;

            object_track.errcov[0][3] = log_object_track.xy_posn_cov;
            object_track.errcov[3][0] = log_object_track.xy_posn_cov;
            object_track.errcov[1][4] = log_object_track.xy_vel_cov;
            object_track.errcov[4][1] = log_object_track.xy_vel_cov;
         }
         else if (F360_TRACKER_TRKFLTR_CCA == log_object_track.trk_fltr_type)
         {
            object_track.errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_X]   = log_object_track.xposn_var;
            object_track.errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_Y]   = log_object_track.yposn_var;
            object_track.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] = log_object_track.xvel_var;
            object_track.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] = log_object_track.yvel_var;
            object_track.errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AX] = log_object_track.xaccel_var;
            object_track.errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AY] = log_object_track.yaccel_var;

            object_track.errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_Y]   = log_object_track.xy_posn_cov;
            object_track.errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_X]   = log_object_track.xy_posn_cov;
            object_track.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY] = log_object_track.xy_vel_cov;
            object_track.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VX] = log_object_track.xy_vel_cov;
            object_track.errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AY] = log_object_track.xy_accel_cov;
            object_track.errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AX] = log_object_track.xy_accel_cov;
         }
         else // CTCA
         {
            object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_X] = log_object_track.xposn_var;
            object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_Y] = log_object_track.yposn_var;
            object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_H] = log_object_track.heading_var;
            object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_C] = log_object_track.curvature_var;
            object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S] = log_object_track.speed_var;
            object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_A] = log_object_track.accel_var;

            object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_Y] = log_object_track.xy_posn_cov;
            object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_X] = log_object_track.xy_posn_cov;
            object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_H] = log_object_track.speed_heading_cov;
            object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_S] = log_object_track.speed_heading_cov;
            object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_C] = log_object_track.accel_curvature_cov;
            object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_A] = log_object_track.accel_curvature_cov;
         }

         object_track.Set_Bbox_Orientation(Angle{log_object_track.orientation});
         object_track.heading_rate = log_object_track.orientation_rate;

         object_track.length_processed = log_object_track.length;
         object_track.width_processed  = log_object_track.width;

         object_track.time_since_measurement    = 0.0F;
         object_track.time_since_track_updated  = 0.0F;
         object_track.time_since_stage_start    = log_object_track.time_since_stage_start;
         object_track.time_since_initialization = log_object_track.time_since_initialization;
         object_track.time_since_downselected   = log_object_track.time_since_downselected;

         object_track.otg_height = log_object_track.otg_height; // Will be overwritten if internals are available as otg_height
                                                                // depends on additional signals in internals
         object_track.confidenceLevel = log_object_track.confidence_level;
         object_track.exist_prob      = log_object_track.existence_probability;
         object_track.average_rcs     = log_object_track.radar_cross_section;

         object_track.ndets           = log_object_track.ndets;
         object_track.trk_fltr_type   = static_cast<F360_Trk_Fltr_Type_T>(log_object_track.trk_fltr_type);
         object_track.reference_point = static_cast<F360_Reference_Point_T>(log_object_track.reference_point);
         object_track.status          = static_cast<F360_Object_Status_T>(log_object_track.status);

         object_track.object_class           = static_cast<F360_Object_Class_T>(log_object_track.object_class);
         object_track.probability_pedestrian = static_cast<float32_t>(log_object_track.probability_pedestrian) / 100.0F;
         object_track.probability_car        = static_cast<float32_t>(log_object_track.probability_car) / 100.0F;
         object_track.probability_motorcycle = static_cast<float32_t>(log_object_track.probability_motorcycle) / 100.0F;
         object_track.probability_bicycle    = static_cast<float32_t>(log_object_track.probability_bicycle) / 100.0F;
         object_track.probability_truck      = static_cast<float32_t>(log_object_track.probability_truck) / 100.0F;
         object_track.probability_undet      = static_cast<float32_t>(log_object_track.probability_undet) / 100.0F;

         object_track.f_moving   = (1U == log_object_track.f_moving);
         object_track.f_moveable = (1U == log_object_track.f_moveable);
         object_track.f_oncoming = (1U == log_object_track.f_oncoming);

         object_track.underdrivable_status_ocg =
            static_cast<ocg::OCG_Underdrivable_Status_T>(log_object_track.underdrivable_status_ocg);
         object_track.probability_underdrivable_ocg =
            static_cast<float32_t>(log_object_track.probability_underdrivable_ocg) / 100.0F;
         object_track.drivable_status_sg     = static_cast<sg::SG_Drivability_Class_T>(log_object_track.drivable_status_sg);
         object_track.drivable_confidence_sg = log_object_track.drivable_confidence_sg;

         object_track.movable_prob     = static_cast<float32_t>(log_object_track.movable_prob) / 100.0F;
         object_track.mirror_prob      = static_cast<float32_t>(log_object_track.mirror_probability) / 100.0F;
         object_track.occlusion_status = static_cast<F360_Occlusion_Status_T>(log_object_track.occlusion_status);
         object_track.aeb_confidence   = static_cast<F360_AEB_Confidence_T>(log_object_track.aeb_confidence);

         /* Post Processing */
         object_track.hdg_ptng_disagmt = Normalize_Heading_Angle(log_object_track.orientation - log_object_track.heading,
                                                                 0.0F); // Will be overwritten if internals are available

         tracker_info.active_obj_ids[tracker_info.num_active_objs] = static_cast<int32_t>(obj_idx) + 1;
         tracker_info.num_active_objs++;

         // Check unique_id
         bool f_unique_id_found = false;
         for (uint32_t j = 0U; j < tracker_info.num_unique_objs; j++)
         {
            f_unique_id_found = f_unique_id_found || (object_track.unique_id == unique_objs[j]);
         }
         if (!f_unique_id_found)
         {
            unique_objs[tracker_info.num_unique_objs] = object_track.unique_id;
            tracker_info.num_unique_objs++;
         }

         // Reduced objects
         if (object_track.reduced_id > 0)
         {
            tracker_info.reduced_active_obj_ids[tracker_info.reduced_num_active_objs] = object_track.reduced_id;
            tracker_info.reduced_num_active_objs++;
            reduced_obj_idx_used[object_track.reduced_id - 1] = true;
         }
      }
   }

   int32_t n_inactive_objects = 0;
   for (int32_t i = 0; i < static_cast<int32_t>(NUMBER_OF_OBJECT_TRACKS); i++)
   {
      if (F360_OBJECT_STATUS_INVALID == objects[i].status)
      {
         tracker_info.inactive_obj_ids[n_inactive_objects] = i + 1;
         n_inactive_objects++;
      }
   }

   int32_t n_inactive_reduced_objects = 0;
   for (int32_t i = 0; i < static_cast<int32_t>(NUMBER_OF_REDUCED_OBJECT_TRACKS); i++)
   {
      if (!reduced_obj_idx_used[i])
      {
         tracker_info.reduced_inactive_obj_ids[n_inactive_reduced_objects] = i + 1;
         n_inactive_reduced_objects++;
      }
   }

   // Compute CIPV and priority for all objects from logged host state
   Update_CIPV_Long_Pos(host, calibrations, tracker_info);
   const float32_t curvature = Map_Curvature_For_Priority(host.curvature_rear);
   const Angle host_sideslip(host.vcs_sideslip);
   Calculate_Priority_For_All_Objects_And_Sort(host.vcs_speed, curvature, host_sideslip, tracker_info, objects);
}

void Populate_Objects_Log_Data(F360_Object_Log_Output_T &objects_log,
                               const F360_Object_Track_T (&objects)[NUMBER_OF_OBJECT_TRACKS],
                               const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS], const F360_Detection_List_T &det_list,
                               const F360_Host_T &host, const F360_Tracker_Info_T &tracker_info)
{
   // Pack the objects
   for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
   {
      const int32_t obj_idx                   = tracker_info.active_obj_ids[i] - 1;
      const F360_Object_Track_T &object_track = objects[obj_idx];
      F360_Object_Log_T &log_object_track     = objects_log.object[i];

      log_object_track.unique_id = object_track.unique_id;
      log_object_track.trkID     = static_cast<uint16_t>(object_track.id);
      log_object_track.reducedID = static_cast<uint16_t>(object_track.reduced_id);

      log_object_track.xposn  = object_track.vcs_position.x;
      log_object_track.yposn  = object_track.vcs_position.y;
      log_object_track.xvel   = object_track.vcs_velocity.longitudinal;
      log_object_track.yvel   = object_track.vcs_velocity.lateral;
      log_object_track.xaccel = object_track.vcs_accel.longitudinal;
      log_object_track.yaccel = object_track.vcs_accel.lateral;

      log_object_track.heading    = object_track.vcs_heading.Value();
      log_object_track.curvature  = object_track.curvature;
      log_object_track.speed      = object_track.speed;
      log_object_track.tang_accel = object_track.tang_accel;

      if (object_track.trk_fltr_type == F360_TRACKER_TRKFLTR_CCA)
      {
         log_object_track.xposn_var  = object_track.errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_X];
         log_object_track.yposn_var  = object_track.errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_Y];
         log_object_track.xvel_var   = object_track.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX];
         log_object_track.yvel_var   = object_track.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY];
         log_object_track.xaccel_var = object_track.errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AX];
         log_object_track.yaccel_var = object_track.errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AY];

         log_object_track.xy_posn_cov  = object_track.errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_Y];
         log_object_track.xy_vel_cov   = object_track.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY];
         log_object_track.xy_accel_cov = object_track.errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AY];

         log_object_track.heading_var         = 0.0F;
         log_object_track.curvature_var       = 0.0F;
         log_object_track.speed_var           = 0.0F;
         log_object_track.accel_var           = 0.0F;
         log_object_track.speed_heading_cov   = 0.0F;
         log_object_track.accel_curvature_cov = 0.0F;
      }
      else
      {
         log_object_track.xposn_var     = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_X];
         log_object_track.yposn_var     = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_Y];
         log_object_track.heading_var   = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_H];
         log_object_track.curvature_var = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_C];
         log_object_track.speed_var     = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S];
         log_object_track.accel_var     = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_A];

         log_object_track.xy_posn_cov         = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_Y];
         log_object_track.speed_heading_cov   = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_H];
         log_object_track.accel_curvature_cov = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_C];
      }

      const float32_t host_longvel = host.speed * F360_Cosf(host.vcs_sideslip);
      const float32_t host_latvel  = host.speed * F360_Sinf(host.vcs_sideslip);
      const float32_t xpos_offset  = (tracker_info.output_coordinate_system == 2U) ? host.dist_rear_axle_to_vcs_m : 0.0F;

      log_object_track.xvel_relative =
         object_track.vcs_velocity.longitudinal - host_longvel + log_object_track.yposn * host.yaw_rate_rad;
      log_object_track.yvel_relative =
         object_track.vcs_velocity.lateral - host_latvel - (log_object_track.xposn + xpos_offset) * host.yaw_rate_rad;
      log_object_track.xaccel_relative = object_track.vcs_accel.longitudinal - host.vcs_long_acceleration -
                                         (log_object_track.xposn + xpos_offset) * host.yaw_rate_rad * host.yaw_rate_rad;
      log_object_track.yaccel_relative = object_track.vcs_accel.lateral - host.vcs_lat_acceleration -
                                         log_object_track.yposn * host.yaw_rate_rad * host.yaw_rate_rad;

      log_object_track.orientation          = object_track.bbox.Get_Orientation().Value();
      log_object_track.orientation_rate     = object_track.heading_rate;
      log_object_track.orientation_var      = object_track.orientation_std;
      log_object_track.orientation_rate_var = Compute_Heading_Rate_Variance(object_track.heading_rate);

      log_object_track.length     = object_track.length_processed;
      log_object_track.width      = object_track.width_processed;
      log_object_track.length_var = object_track.length_uncertainty;
      log_object_track.width_var  = object_track.width_uncertainty;

      log_object_track.time_since_initialization = object_track.time_since_initialization;
      log_object_track.time_since_downselected   = object_track.time_since_downselected;
      log_object_track.time_since_stage_start    = object_track.time_since_stage_start;

      log_object_track.otg_height            = object_track.otg_height;
      log_object_track.confidence_level      = object_track.confidenceLevel;
      log_object_track.existence_probability = object_track.exist_prob;
      log_object_track.radar_cross_section   = object_track.average_rcs;

      log_object_track.sensor_src = Compute_Object_Sensor_Source(sensors, object_track, det_list);

      log_object_track.ndets              = static_cast<uint8_t>(object_track.ndets);
      log_object_track.num_rr_inlier_dets = static_cast<uint8_t>(object_track.num_rr_inlier_dets);
      log_object_track.trk_fltr_type      = static_cast<uint8_t>(object_track.trk_fltr_type);
      log_object_track.reference_point    = static_cast<uint8_t>(object_track.reference_point);
      log_object_track.status             = static_cast<uint8_t>(object_track.status);

      log_object_track.object_class           = static_cast<uint8_t>(object_track.object_class);
      float32_t temp_prob                     = object_track.probability_pedestrian * 100.0F + 0.5F;
      log_object_track.probability_pedestrian = static_cast<uint8_t>(temp_prob);
      temp_prob                               = object_track.probability_car * 100.0F + 0.5F;
      log_object_track.probability_car        = static_cast<uint8_t>(temp_prob);
      temp_prob                               = object_track.probability_motorcycle * 100.0F + 0.5F;
      log_object_track.probability_motorcycle = static_cast<uint8_t>(temp_prob);
      temp_prob                               = object_track.probability_bicycle * 100.0F + 0.5F;
      log_object_track.probability_bicycle    = static_cast<uint8_t>(temp_prob);
      temp_prob                               = object_track.probability_truck * 100.0F + 0.5F;
      log_object_track.probability_truck      = static_cast<uint8_t>(temp_prob);
      temp_prob                               = object_track.probability_undet * 100.0F + 0.5F;
      log_object_track.probability_undet      = static_cast<uint8_t>(temp_prob);

      log_object_track.f_moving      = object_track.f_moving ? 1U : 0U;
      log_object_track.f_moveable    = object_track.f_moveable ? 1U : 0U;
      log_object_track.f_oncoming    = object_track.f_oncoming ? 1U : 0U;
      log_object_track.f_onguardrail = (F360_INVALID_UNSIGNED_ID == object_track.on_sep_id) ? 0U : 1U;

      temp_prob                                      = object_track.probability_underdrivable_ocg * 100.0F + 0.5F;
      log_object_track.probability_underdrivable_ocg = static_cast<uint8_t>(temp_prob);
      log_object_track.underdrivable_status_ocg      = static_cast<uint8_t>(object_track.underdrivable_status_ocg);
      log_object_track.drivable_status_sg            = static_cast<uint8_t>(object_track.drivable_status_sg);
      log_object_track.drivable_confidence_sg        = object_track.drivable_confidence_sg;

      log_object_track.obstacle_prob      = 100U; // Hard-coded to 100%
      temp_prob                           = object_track.movable_prob * 100.0F + 0.5F;
      log_object_track.movable_prob       = static_cast<uint8_t>(temp_prob);
      temp_prob                           = object_track.mirror_prob * 100.0F + 0.5F;
      log_object_track.mirror_probability = static_cast<uint8_t>(temp_prob);
      log_object_track.occlusion_status   = static_cast<uint8_t>(object_track.occlusion_status);
      log_object_track.aeb_confidence     = static_cast<uint8_t>(object_track.aeb_confidence);

      if (objects_log.f360header.coordinate_system == 2U) // 0 = undefined, 1 = SAE, 2 = ISO
      {
         log_object_track.xposn           = log_object_track.xposn + host.dist_rear_axle_to_vcs_m;
         log_object_track.yposn           = -log_object_track.yposn;
         log_object_track.yvel            = -log_object_track.yvel;
         log_object_track.yvel_relative   = -log_object_track.yvel_relative;
         log_object_track.yaccel          = -log_object_track.yaccel;
         log_object_track.yaccel_relative = -log_object_track.yaccel_relative;

         log_object_track.heading   = -log_object_track.heading;
         log_object_track.curvature = -log_object_track.curvature;

         log_object_track.orientation      = -log_object_track.orientation;
         log_object_track.orientation_rate = -log_object_track.orientation_rate;

         log_object_track.xy_posn_cov         = -log_object_track.xy_posn_cov;
         log_object_track.xy_vel_cov          = -log_object_track.xy_vel_cov;
         log_object_track.xy_accel_cov        = -log_object_track.xy_accel_cov;
         log_object_track.speed_heading_cov   = -log_object_track.speed_heading_cov;
         log_object_track.accel_curvature_cov = -log_object_track.accel_curvature_cov;
      }
   }
}

/*===========================================================================*\
* FUNCTION: Compute_Heading_Rate_Variance()
*===========================================================================
* ABSTRACT:
* Function computing the heading rate variance from a given heading rate
\*===========================================================================*/
static float32_t Compute_Heading_Rate_Variance(const float32_t heading_rate)
{
   const float32_t smallest_heading_rate_calib     = 0.000F; // 0 deg/s
   const float32_t largest_heading_rate_calib      = 0.698F; // 40 deg/s
   const float32_t smallest_heading_rate_std_calib = 0.052F; // 3 deg/s
   const float32_t largest_heading_rate_std_calib  = 0.209F; // 12 deg/s

   const float32_t std =
      F360_Linear_Equation_With_Saturation(std::abs(heading_rate), smallest_heading_rate_calib, largest_heading_rate_calib,
                                           smallest_heading_rate_std_calib, largest_heading_rate_std_calib);

   const float32_t var = std * std;

   return var;
}

/*===========================================================================*\
* FUNCTION: Compute_Object_Sensor_Source()
*===========================================================================
* ABSTRACT:
* Function computing
\*===========================================================================*/
static uint16_t Compute_Object_Sensor_Source(const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
                                             const F360_Object_Track_T &object_track, const F360_Detection_List_T &det_list)
{
   uint16_t status_sensor_temp       = 0x0000U;
   const uint16_t front_mid_sensor   = 0x0001U;
   const uint16_t front_left_sensor  = 0x0002U;
   const uint16_t left_side_sensor   = 0x0004U;
   const uint16_t rear_left_sensor   = 0x0008U;
   const uint16_t rear_mid_sensor    = 0x0010U;
   const uint16_t rear_right_sensor  = 0x0020U;
   const uint16_t right_side_sensor  = 0x0040U;
   const uint16_t front_right_sensor = 0x0080U;
   for (uint32_t i = 0U; i < object_track.ndets; ++i)
   {
      const uint32_t det_index = object_track.detids[i] - 1U;
      const uint8_t sID        = static_cast<uint8_t>(det_list.detections[det_index].sensor_id);
      if ((sID > 0U) && ((object_track.status == F360_OBJECT_STATUS_NEW) || (object_track.status == F360_OBJECT_STATUS_UPDATED)))
      {
         uint16_t current_sensor;
         switch (sensors[sID - 1U].mounting_location)
         {
            case F360_MOUNTING_LOCATION_CENTER_FORWARD:
            case F360_MOUNTING_LOCATION_CENTER2_FORWARD:
            case F360_MOUNTING_LOCATION_CENTER3_FORWARD:
               current_sensor = front_mid_sensor;
               break;
            case F360_MOUNTING_LOCATION_LEFT_FORWARD:
               current_sensor = front_left_sensor;
               break;
            case F360_MOUNTING_LOCATION_LEFT_SIDE1:
            case F360_MOUNTING_LOCATION_LEFT_SIDE2:
               current_sensor = left_side_sensor;
               break;
            case F360_MOUNTING_LOCATION_LEFT_REAR:
               current_sensor = rear_left_sensor;
               break;
            case F360_MOUNTING_LOCATION_CENTER_REAR:
            case F360_MOUNTING_LOCATION_CENTER2_REAR:
            case F360_MOUNTING_LOCATION_CENTER3_REAR:
               current_sensor = rear_mid_sensor;
               break;
            case F360_MOUNTING_LOCATION_RIGHT_REAR:
               current_sensor = rear_right_sensor;
               break;
            case F360_MOUNTING_LOCATION_RIGHT_SIDE1:
            case F360_MOUNTING_LOCATION_RIGHT_SIDE2:
               current_sensor = right_side_sensor;
               break;
            case F360_MOUNTING_LOCATION_RIGHT_FORWARD:
               current_sensor = front_right_sensor;
               break;
            case F360_MOUNTING_LOCATION_UNKNOWN:
            default:
               current_sensor = 0U;
               break;
         }
         status_sensor_temp = status_sensor_temp | current_sensor;
      }
   }
   return status_sensor_temp;
}
} // namespace f360_variant_A
