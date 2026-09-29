#include "sg_calibrations.h"

namespace sg
{
   Calibrations_T::Calibrations_T()
   {
      // elapsed time
      elapsed_time_th = 50000U; // [us]

      // host properties
      host_position.x    = 0.0F;
      host_position.y    = 0.0F;
      host_length        = 4.2F;
      host_width         = 1.8F;
      host_nominal_speed = 0.0F;

      // time_update_detections
      process_noise.host_speed_weight_lat  = 0.01F;
      process_noise.host_speed_weight_lon  = 0.01F;
      process_noise.process_noise_base_std = 0.01F;
      process_noise.host_speed_weight      = 0.01F;

      // det_sort_params
      det_sort_params.bin_bounds_max = 250.0F;
      det_sort_params.bin_bounds_min = 0.0F;
      det_sort_params.bin_width      = 1.0F;

      // downselect_input_detections
      max_rr_compens_for_stat_det                       = 5.0F;
      nondrivable_view_range.lateral.min                = -40.0F;
      nondrivable_view_range.lateral.max                = 40.0F;
      nondrivable_view_range.longitudinal.min           = -10.0F;
      nondrivable_view_range.longitudinal.max           = 80.0F;
      nondrivable_view_range.underground_vertical.min   = -3.0F;
      nondrivable_view_range.underground_vertical.max   = -0.0F;
      nondrivable_view_range.overground_vertical.min    = 0.0F;
      nondrivable_view_range.overground_vertical.max    = 3.0F;
      overdrivable_view_range.lateral.min               = -40.0F;
      overdrivable_view_range.lateral.max               = 40.0F;
      overdrivable_view_range.longitudinal.min          = -10.0F;
      overdrivable_view_range.longitudinal.max          = 80.0F;
      overdrivable_view_range.underground_vertical.min  = -0.2F;
      overdrivable_view_range.underground_vertical.max  = -0.0F;
      overdrivable_view_range.overground_vertical.min   = 0.0F;
      overdrivable_view_range.overground_vertical.max   = 0.2F;
      underdrivable_view_range.lateral.min              = -40.0F;
      underdrivable_view_range.lateral.max              = 40.0F;
      underdrivable_view_range.longitudinal.min         = -10.0F;
      underdrivable_view_range.longitudinal.max         = 80.0F;
      underdrivable_view_range.underground_vertical.min = -15.0F;
      underdrivable_view_range.underground_vertical.max = -4.0F;
      underdrivable_view_range.overground_vertical.min  = 4.0F;
      underdrivable_view_range.overground_vertical.max  = 15.0F;
      accepted_probability_level                        = 0.25F;
      maximum_valid_azimuth_confidence_value            = 2;
      max_host_speed_for_poor_azim_confid_det           = 2.0F;
      max_distance_for_poor_azim_confid_det             = 3.0F;

      // importance
      importance_calibrations.distance_factors[0U]          = 0.001F;
      importance_calibrations.distance_factors[1U]          = 0.005F;
      importance_calibrations.distance_factors[2U]          = 0.015F;
      importance_calibrations.shape_correction_factors[0U]  = 0.01F;
      importance_calibrations.shape_correction_factors[1U]  = 0.0002F;
      importance_calibrations.age_impact_factor             = 0.25F;
      importance_calibrations.distance_impact_factor        = 1.0F;
      importance_calibrations.host_speed_impact_factor      = 0.1F;
      importance_calibrations.use_existence_probability     = false;
      importance_calibrations.max_associated_det_vertex_age = 3U;
      importance_calibrations.max_det_age                   = 6U;

      // to be used later in downselection
      curvi_roi_curvature_threshold = 0.001F;
      roi_front_length_vcs          = 80.0F;
      roi_half_width_vcs            = 10.0F;
      roi_rear_length_vcs           = 20.0F;

      // combine_detections
      R_position_covariance_x  = 0.8F;
      R_position_covariance_y  = 0.8F;
      R_position_covariance_xy = 0.0F;

      // squeeze_detections
      lat_linear_piecewise_transform_coefs[0U].squeeze_range  = 10.0F;
      lat_linear_piecewise_transform_coefs[0U].squeeze_factor = 1.0F;
      lat_linear_piecewise_transform_coefs[1U].squeeze_range  = 15.0F;
      lat_linear_piecewise_transform_coefs[1U].squeeze_factor = 1.0F;
      lat_linear_piecewise_transform_coefs[2U].squeeze_range  = 20.0F;
      lat_linear_piecewise_transform_coefs[2U].squeeze_factor = 1.0F;

      lon_linear_piecewise_transform_coefs[0U].squeeze_range  = 30.0F;
      lon_linear_piecewise_transform_coefs[0U].squeeze_factor = 1.0F;
      lon_linear_piecewise_transform_coefs[1U].squeeze_range  = 60.0F;
      lon_linear_piecewise_transform_coefs[1U].squeeze_factor = 1.0F;
      lon_linear_piecewise_transform_coefs[2U].squeeze_range  = 100.0F;
      lon_linear_piecewise_transform_coefs[2U].squeeze_factor = 0.8F;

      lat_linear_piecewise_transform_coefs_underdrivable[0U].squeeze_range  = 66.0F;
      lat_linear_piecewise_transform_coefs_underdrivable[0U].squeeze_factor = 0.4F;
      lat_linear_piecewise_transform_coefs_underdrivable[1U].squeeze_range  = 120.0F;
      lat_linear_piecewise_transform_coefs_underdrivable[1U].squeeze_factor = 0.2F;
      lat_linear_piecewise_transform_coefs_underdrivable[2U].squeeze_range  = 200.0F;
      lat_linear_piecewise_transform_coefs_underdrivable[2U].squeeze_factor = 0.1F;

      lon_linear_piecewise_transform_coefs_underdrivable[0U].squeeze_range  = 66.0F;
      lon_linear_piecewise_transform_coefs_underdrivable[0U].squeeze_factor = 0.6F;
      lon_linear_piecewise_transform_coefs_underdrivable[1U].squeeze_range  = 170.0F;
      lon_linear_piecewise_transform_coefs_underdrivable[1U].squeeze_factor = 0.4F;
      lon_linear_piecewise_transform_coefs_underdrivable[2U].squeeze_range  = 200.0F;
      lon_linear_piecewise_transform_coefs_underdrivable[2U].squeeze_factor = 0.4F;

      // cluster_detections
      cluster_radius                             = 1.1F;
      cluster_radius_underdrivable               = 2.9F;
      min_cluster_points                         = 4U;
      min_cluster_points_underdrivable           = 2U;
      min_existence_probability                  = 0.7F;
      historical_num_neighbors_forgetting_factor = 0.9F;

      // compute_existence_probability
      neighbors_factor = 0.9F;
      age_factor       = 0.1F;
      alpha            = 6.0F;

      // associate_detections_to_contour
      assoc_width_min                  = 2.6F;
      assoc_d_length_min               = 0.0F;
      assoc_width_max                  = 5.9F;
      assoc_d_length_max               = 1.6F;
      assoc_d_length_ending            = 1.0F;
      assoc_dynamic_gates              = false;
      det_seg_assignment_threshold     = 1.0F;
      min_dets_assoc_update_cluster_id = 1U;
      vertex_association_impact_type   = Vertex_Association_Impact_Type_T::PROPORTIONAL;

      // associate_cluster_boundary_detections_to_contour
      assoc_width_extension_min         = 2.5F;
      assoc_width_extension_max         = 6.0F;
      assoc_extended_meas_covariance_xx = 0.1F;
      assoc_extended_meas_covariance_yy = 0.1F;

      // measurement update
      length_measurement_noise    = 0.8F;
      min_dets_for_shrinking      = 1U;
      position_forgetting_factor  = 1.0F;
      length_forgetting_factor    = 0.98F;
      minimal_length_spread       = 0.1F;
      max_num_ending_segment_dets = static_cast<uint8_t>(SG_MAX_NUM_ENDING_SEGMENT_DETS);

      // initialize_contours
      init_look_distance                         = 10.0F;
      init_look_distance_underdrivable           = 18.0F;
      min_segment_length                         = 0.5F;
      sub_cluster_radius                         = 2.9F;
      sub_min_cluster_points                     = 2U;
      min_dets_for_contour_init                  = 2U;
      min_dets_for_first_contour_init            = 2U;
      max_num_init_contoured_clusters            = 255U;
      initial_reliability                        = 3.0F;
      min_num_dets_for_stationary_hypothesis     = 80U;
      min_dets_span_for_stationary_hypothesis    = 30.0F;
      min_num_vertices_for_stationary_hypothesis = 10U;
      sensor_look_diversity_threshold            = 0.7F;

      // init_contour_covariance
      init_vertex_covariance = 0.8F;

      // get_unoccluded_dets_mask
      init_azimuth_margin = 0.0F;
      azimuth_epsilon     = 1.0E-5F;
      range_margin        = 5.2F;

      // get_occluded_contours
      contour_occlusion_threshold = 0.5F;
      occluders_num_threshold     = 0U;

      // get_occluded_vertices
      contour_occlusion_azimuth_margin   = 0.0F;
      contour_occlusion_range_margin[0U] = -0.5F;
      contour_occlusion_range_margin[1U] = 6.0F;

      // merge_contours
      merge_distance_threshold = 3.0F;
      merge_max_angle          = 60.0F;

      // split_and_merge_forks
      fork_distance_merge_threshold      = 0.01F;
      fork_alignment_angle_threshold[0U] = 10.0F;
      fork_alignment_angle_threshold[1U] = 100.0F;

      // presimplify_contours
      presimplify_host_length = 3.0F;
      presimplify_type        = 2U;
      presimplify_min_angle   = 30.0F;
      presimplify_max_angle   = 15.0F;

      // simplify_contours
      simplify_min_turning_angle = 30.0F;
      simplify_type              = 1U;
      simplify_length_range[0U]  = 3.0F;
      simplify_length_range[1U]  = 10.0F;
      simplify_angle             = 30.0F;

      // remove_vertices
      remove_vertices_front_limit = 120.0F;
      remove_vertices_rear_limit  = -10.0F;
      remove_vertices_left_limit  = -40.0F;
      remove_vertices_right_limit = 40.0F;
      accepted_uncertainty        = 0.81F;
      max_num_cycles_no_update    = 100U;

      // critical region for drivability classification
      create_subsegments                          = true;
      critical_region_polygon[0U].x               = 0.0F;
      critical_region_polygon[1U].x               = 180.0F;
      critical_region_polygon[2U].x               = 180.0F;
      critical_region_polygon[3U].x               = 0.0F;
      critical_region_polygon[0U].y               = -10.0F;
      critical_region_polygon[1U].y               = -20.0F;
      critical_region_polygon[2U].y               = 20.0F;
      critical_region_polygon[3U].y               = 10.0F;
      critical_region_subdivisions                = 3U;
      critical_region_min_abs_curvature_threshold = 0.0001F;
      critical_region_max_abs_curvature_threshold = 0.02F;
      subsegment_length                           = 1.5F;
      projection_distance_threshold               = 0.2F;
      angle_between_segments_threshold            = 0.26F;

      // subsegments classification
      min_num_dets_for_classification                 = 6.0F;
      overdrivable_max_z                              = 0.2F;
      underdrivable_min_z                             = 3.0F;
      underdrivable_min_z_std                         = 4.0F;
      decision_tree_obstacle_prob_thr                 = 0.5F;
      decision_tree_overdrivability_obstacle_prob_thr = 0.5F;
      classify_subsegments                            = true;

      // features calculation
      features_inliers_threshold               = 0.25F;
      features_radar_height                    = 0.0F;
      features_z_max_threshold                 = 999.0F;
      features_z_min_threshold                 = -999.0F;
      features_min_rr_comp_of_stat_dealias_det = 5.0F;
      features_max_azimuth_confidence          = 3;
      features_max_elevation_confidence        = 3;

      // select and remove excess
      max_num_contoured_clusters = 254U;

      // contour downselection
      contour_priority_shift_x             = 0.0F;
      contour_priority_compaction_factor_x = 1.0F;
      contour_priority_compaction_factor_y = 1.0F;

      // merge sg and dc contours
      max_num_vertices_per_fused_contour = 100U;
      max_num_fused_contours             = 100U;
      max_num_fused_vertices             = 400U;
   }
}
