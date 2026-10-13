/*===================================================================================*\
 * FILE:  f360_calculate_priority.h
 *====================================================================================
 * Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose."
 *-----------------------------------------------------------------------------------------
 * DESCRIPTION:
 * This file contains function signatures to compute cluster and object priorities.
 *
 *
 * Applicable Standards (in order of precedence: highest first):
 * ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
 * ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
 ***/

#ifndef F360_CALCULATE_PRIORITY_H
#define F360_CALCULATE_PRIORITY_H

#include "f360_angle.h"
#include "f360_calibrations.h"
#include "f360_cluster.h"
#include "f360_detection_list.h"
#include "f360_detection_props.h"
#include "f360_host.h"
#include "f360_object_track.h"
#include "f360_reuse.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
struct F360_Priority_Pos_Vel_T
{
   float32_t vcs_pos_x;
   float32_t vcs_pos_y;
   float32_t vcs_vel_long;
   float32_t vcs_vel_lat;
   float32_t movable_prob;
};

struct Priority_Zone_Params_T
{
   float32_t polar_zone_radius;          // Speed-dependent inner radius where range priority is 1.0
   float32_t polar_zone_range_step;      // Transition band width for range decay (30% of polar_zone_radius)
   float32_t polar_zone_outer_radius;    // Outer edge of transition band (polar_zone_radius + polar_zone_range_step)
   float32_t polar_zone_az_sin;          // Sin of the speed-dependent half-angle azimuth boundary
   float32_t polar_zone_az_cos;          // Cos of the speed-dependent half-angle azimuth boundary
   float32_t polar_zone_az_threshold;    // Cross-product threshold for azimuth transition band
   float32_t ellipse_extend_long_factor; // Speed-dependent lateral stretch for elliptical distance metric
   float32_t corridor_default_range;     // Speed-dependent default corridor range
};

void Update_Priority_Zone_Params(const float32_t host_speed, const float32_t cipv_long_pos);

float32_t Calculate_Distance_Priority(const float32_t longitudinal_pos, const float32_t curvi_lat_pos);

float32_t Calculate_Front_Polar_Zone_Priority(const float32_t range, const float32_t longitudinal_pos,
                                              const float32_t curvi_lat_pos);

float32_t Calculate_Corridor_Priority(const float32_t longitudinal_pos, const float32_t curvi_lat_pos);

float32_t Calculate_Position_Priority(const float32_t longitudinal_pos, const float32_t curvi_lat_pos, const float32_t range);

float32_t Compute_TTC_2D(const float32_t vcs_pos_x, const float32_t vcs_pos_y, const float32_t rel_vel_x,
                         const float32_t rel_vel_y, const float32_t range);

float32_t Compute_TTC_1D(const float32_t range, const float32_t rdot_relative);

float32_t Calculate_Object_TTC_Priority(const F360_Priority_Pos_Vel_T &pos_vel, const float32_t host_speed,
                                        const Angle &host_sideslip, const float32_t range, const float32_t curvi_lat_pos);

float32_t Calculate_Priority_From_Pos_Vel(const F360_Priority_Pos_Vel_T &pos_vel, const float32_t host_speed,
                                          const Angle &host_sideslip, const float32_t host_curvature);

float32_t Calculate_Priority_For_Object(const F360_Object_Track_T &obj_trk, const float32_t host_speed, const Angle &host_sideslip,
                                        const float32_t host_curvature);

float32_t Apply_Conservative_Movable_Bonus(const float32_t base_priority, const bool f_moving,
                                           const F360_Tracker_Info_T &tracker_info);

bool Is_Cluster_Moving(const F360_Cluster_T &cluster);

float32_t Map_TTC_To_Priority(const float32_t ttc);

float32_t Compute_CPA_Attenuation(const float32_t vcs_pos_x, const float32_t vcs_pos_y, const float32_t rel_vel_x,
                                  const float32_t rel_vel_y, const float32_t range);

float32_t Stationary_Lateral_Attenuation(const float32_t curvi_lat_pos);

float32_t Calculate_Priority_For_Cluster(const F360_Cluster_T &cluster, const float32_t host_speed, const float32_t host_curvature,
                                         const Angle &host_sideslip, const F360_Tracker_Info_T &tracker_info);

float32_t Calculate_Cluster_TTC_Priority(const F360_Cluster_T &cluster, const float32_t host_speed, const Angle &host_sideslip,
                                         const float32_t range, const float32_t curvi_lat_pos, const bool f_moving);

void Calculate_Priority_For_All_Clusters(const float32_t host_speed, const float32_t host_curvature, const Angle &host_sideslip,
                                         const F360_Tracker_Info_T &tracker_info, F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS]);

void Apply_Movable_Bonus_For_All_Objects(const int32_t (&movable_indices)[NUMBER_OF_OBJECT_TRACKS], const int32_t num_movable,
                                         const uint32_t max_num_reduced_tracks,
                                         F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]);

float32_t Compute_Detection_Position_Priority(const F360_Detection_T &detection, const F360_Detection_Props_T &det_prop,
                                              const float32_t host_speed, const float32_t cipv_long_pos,
                                              const float32_t host_curvature);

void Calculate_And_Apply_Movable_Bonus(const int32_t rank, const uint32_t max_num_reduced_tracks, float32_t &priority);

float32_t Calculate_Priority_For_Init(const float32_t host_speed, const Angle &host_sideslip, const F360_Cluster_T &cluster,
                                      const float32_t longvel_estimate, const float32_t latvel_estimate,
                                      const float32_t speed_thresh, const float32_t host_curvature,
                                      const F360_Tracker_Info_T &tracker_info);

float32_t Map_Curvature_For_Priority(const float32_t host_curvature);

void Calculate_Priority_For_All_Objects_And_Sort(const float32_t host_speed, const float32_t host_curvature,
                                                 const Angle &host_sideslip, F360_Tracker_Info_T &tracker_info,
                                                 F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]);

void Adjust_Priority_On_Motion_Flip(const F360_Host_T &host, const bool f_became_movable, F360_Object_Track_T &object,
                                    F360_Tracker_Info_T &tracker_info);
} // namespace f360_variant_A
#endif
