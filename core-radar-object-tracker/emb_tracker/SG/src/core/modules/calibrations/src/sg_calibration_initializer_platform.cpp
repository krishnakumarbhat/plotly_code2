#include "sg_calibration_initializer_platform.h"

#include "sg_variant.h"

namespace sg
{
void CalibrationInitializerPlatform::initialize(CommonCalibs &calibrations, const Scenario &chosen_scenario) const
{
   calibrations.elapsed_time_th                                    = 50'000U;
   calibrations.r_position_covariance.x                            = 0.8F;
   calibrations.r_position_covariance.y                            = 0.8F;
   calibrations.r_position_covariance.xy                           = 0.0F;
   calibrations.view_ranges.nondrivable.lateral.min                = -40.0F;
   calibrations.view_ranges.nondrivable.lateral.max                = 40.0F;
   calibrations.view_ranges.nondrivable.longitudinal.min           = -10.0F;
   calibrations.view_ranges.nondrivable.longitudinal.max           = set_calibration(chosen_scenario, 80.0F, 80.0F, 180.0F);
   calibrations.view_ranges.nondrivable.vertical.overground.min    = 0.0F;
   calibrations.view_ranges.nondrivable.vertical.overground.max    = 3.0F;
   calibrations.view_ranges.nondrivable.vertical.underground.min   = -3.0F;
   calibrations.view_ranges.nondrivable.vertical.underground.max   = -0.0F;
   calibrations.view_ranges.overdrivable.lateral.min               = -40.0F;
   calibrations.view_ranges.overdrivable.lateral.max               = 40.0F;
   calibrations.view_ranges.overdrivable.longitudinal.min          = -10.0F;
   calibrations.view_ranges.overdrivable.longitudinal.max          = set_calibration(chosen_scenario, 80.0F, 80.0F, 180.0F);
   calibrations.view_ranges.overdrivable.vertical.overground.min   = 0.0F;
   calibrations.view_ranges.overdrivable.vertical.overground.max   = 0.2F;
   calibrations.view_ranges.overdrivable.vertical.underground.min  = -0.2F;
   calibrations.view_ranges.overdrivable.vertical.underground.max  = -0.0F;
   calibrations.view_ranges.underdrivable.lateral.min              = -40.0F;
   calibrations.view_ranges.underdrivable.lateral.max              = 40.0F;
   calibrations.view_ranges.underdrivable.longitudinal.min         = -10.0F;
   calibrations.view_ranges.underdrivable.longitudinal.max         = set_calibration(chosen_scenario, 80.0F, 80.0F, 180.0F);
   calibrations.view_ranges.underdrivable.vertical.overground.min  = 4.0F;
   calibrations.view_ranges.underdrivable.vertical.overground.max  = 15.0F;
   calibrations.view_ranges.underdrivable.vertical.underground.min = -15.0F;
   calibrations.view_ranges.underdrivable.vertical.underground.max = -4.0F;
   calibrations.host_position.x                                    = 0.0F;
   calibrations.host_position.y                                    = 0.0F;
   calibrations.min_segment_length                                 = 0.5F;
   calibrations.initial_reliability                                = 3.0F;
   calibrations.azimuth_epsilon                                    = 1.0E-5F;
}

void CalibrationInitializerPlatform::initialize(TimeUpdateCalibs &calibrations, const Scenario &chosen_scenario) const
{
   (void)chosen_scenario; // MISRA
   calibrations.noise.host_speed_weight_lat  = 0.01F;
   calibrations.noise.host_speed_weight_lon  = 0.01F;
   calibrations.noise.process_noise_base_std = 0.01F;
   calibrations.noise.host_speed_weight      = 0.01F;
}

void CalibrationInitializerPlatform::initialize(DetectionProcessingCalibs &calibrations, const Scenario &chosen_scenario) const
{
   (void)chosen_scenario; // MISRA
   calibrations.downselect_input_detections.max_rr_compens_for_stat_det             = 5.0F;
   calibrations.downselect_input_detections.accepted_probability_level              = 0.25F;
   calibrations.downselect_input_detections.max_host_speed_for_poor_azim_confid_det = 2.0F;
   calibrations.downselect_input_detections.max_distance_for_poor_azim_confid_det   = 3.0F;
   calibrations.downselect_input_detections.maximum_valid_azimuth_confidence_value  = 2;
   calibrations.importance_calibrations.distance_factors[0U]                        = 0.001F;
   calibrations.importance_calibrations.distance_factors[1U]                        = 0.005F;
   calibrations.importance_calibrations.distance_factors[2U]                        = 0.015F;
   calibrations.importance_calibrations.shape_correction_factors[0U]                = 0.01F;
   calibrations.importance_calibrations.shape_correction_factors[1U]                = 0.0002F;
   calibrations.importance_calibrations.age_impact_factor                           = 0.25F;
   calibrations.importance_calibrations.distance_impact_factor                      = 1.0F;
   calibrations.importance_calibrations.host_speed_impact_factor                    = 0.1F;
   calibrations.importance_calibrations.use_existence_probability                   = false;
   calibrations.importance_calibrations.max_associated_det_vertex_age               = 3U;
   calibrations.importance_calibrations.max_det_age                                 = 6U;
}

void CalibrationInitializerPlatform::initialize(DetectionClusteringCalibs &calibrations, const Scenario &chosen_scenario) const
{
   calibrations.linear_piecewise_transform_coefficients.lateral.nondrivable[0U].range  = 10.0F;
   calibrations.linear_piecewise_transform_coefficients.lateral.nondrivable[0U].factor = 1.0F;
   calibrations.linear_piecewise_transform_coefficients.lateral.nondrivable[1U].range  = 15.0F;
   calibrations.linear_piecewise_transform_coefficients.lateral.nondrivable[1U].factor = 1.0F;
   calibrations.linear_piecewise_transform_coefficients.lateral.nondrivable[2U].range  = 20.0F;
   calibrations.linear_piecewise_transform_coefficients.lateral.nondrivable[2U].factor = 1.0F;

   calibrations.linear_piecewise_transform_coefficients.longitudinal.nondrivable[0U].range =
      set_calibration(chosen_scenario, 30.0F, 30.0F, 66.0F);
   calibrations.linear_piecewise_transform_coefficients.longitudinal.nondrivable[0U].factor =
      set_calibration(chosen_scenario, 1.0F, 1.0F, 0.3F);
   calibrations.linear_piecewise_transform_coefficients.longitudinal.nondrivable[1U].range =
      set_calibration(chosen_scenario, 60.0F, 60.0F, 170.0F);
   calibrations.linear_piecewise_transform_coefficients.longitudinal.nondrivable[1U].factor =
      set_calibration(chosen_scenario, 1.0F, 1.0F, 0.2F);
   calibrations.linear_piecewise_transform_coefficients.longitudinal.nondrivable[2U].range =
      set_calibration(chosen_scenario, 100.0F, 100.0F, 200.0F);
   calibrations.linear_piecewise_transform_coefficients.longitudinal.nondrivable[2U].factor =
      set_calibration(chosen_scenario, 0.8F, 0.8F, 0.1F);

   calibrations.linear_piecewise_transform_coefficients.lateral.underdrivable[0U].range  = 66.0F;
   calibrations.linear_piecewise_transform_coefficients.lateral.underdrivable[0U].factor = 0.4F;
   calibrations.linear_piecewise_transform_coefficients.lateral.underdrivable[1U].range  = 120.0F;
   calibrations.linear_piecewise_transform_coefficients.lateral.underdrivable[1U].factor = 0.2F;
   calibrations.linear_piecewise_transform_coefficients.lateral.underdrivable[2U].range  = 200.0F;
   calibrations.linear_piecewise_transform_coefficients.lateral.underdrivable[2U].factor = 0.1F;

   calibrations.linear_piecewise_transform_coefficients.longitudinal.underdrivable[0U].range  = 66.0F;
   calibrations.linear_piecewise_transform_coefficients.longitudinal.underdrivable[0U].factor = 0.6F;
   calibrations.linear_piecewise_transform_coefficients.longitudinal.underdrivable[1U].range  = 170.0F;
   calibrations.linear_piecewise_transform_coefficients.longitudinal.underdrivable[1U].factor = 0.4F;
   calibrations.linear_piecewise_transform_coefficients.longitudinal.underdrivable[2U].range  = 200.0F;
   calibrations.linear_piecewise_transform_coefficients.longitudinal.underdrivable[2U].factor = 0.4F;

   calibrations.cluster_detections.existence_probability.neighbors_factor     = 0.9F;
   calibrations.cluster_detections.existence_probability.age_factor           = 0.1F;
   calibrations.cluster_detections.existence_probability.alpha                = 6.0F;
   calibrations.cluster_detections.cluster_radius_nondrivable                 = 1.1F;
   calibrations.cluster_detections.cluster_radius_underdrivable               = 1.1F;
   calibrations.cluster_detections.historical_num_neighbors_forgetting_factor = 0.9F;
   calibrations.cluster_detections.min_cluster_points_nondrivable             = 4U;
   calibrations.cluster_detections.min_cluster_points_underdrivable           = 2U;
}

void CalibrationInitializerPlatform::initialize(MeasurementAssociationCalibs &calibrations, const Scenario &chosen_scenario) const
{
   calibrations.width_min                        = 1.8F;
   calibrations.width_max                        = 3.0F;
   calibrations.length_margin_min                = 1.0F;
   calibrations.length_margin_max                = 1.6F;
   calibrations.length_margin_ending             = set_calibration(chosen_scenario, 1.0F, 1.0F, 1.5F);
   calibrations.f_dynamic_gates                  = true;
   calibrations.min_dets_assoc_update_cluster_id = 1U;

   // associate_cluster_boundary_detections_to_contour
   calibrations.width_extension_min         = 1.3F;
   calibrations.width_extension_max         = 2.5F;
   calibrations.extended_meas_covariance_xx = 0.1F;
   calibrations.extended_meas_covariance_yy = 0.1F;

   calibrations.dynamic_gate_lower_distance = 40.0F;
   calibrations.dynamic_gate_upper_distance = 100.0F;
}

void CalibrationInitializerPlatform::initialize(MeasurementUpdateCalibs &calibrations, const Scenario &chosen_scenario) const
{
   calibrations.length_measurement_noise    = 0.8F;
   calibrations.length_forgetting_factor    = 0.98F;
   calibrations.minimal_length_spread       = 0.1F;
   calibrations.position_forgetting_factor  = 0.995F;
   calibrations.regularity_weight           = set_calibration(chosen_scenario, 0.8F, 0.8F, 5.0F);
   calibrations.regularity_threshold        = 0.1F;
   calibrations.min_dets_for_shrinking      = 1U;
   calibrations.max_num_ending_segment_dets = static_cast<uint8_t>(SG_MAX_NUM_ENDING_SEGMENT_DETS);
}

void CalibrationInitializerPlatform::initialize(ContourInitializationCalibs &calibrations, const Scenario &chosen_scenario) const
{
   calibrations.cluster_priority_shift_x             = set_calibration(chosen_scenario, 0.0F, 5.0F, 40.0F);
   calibrations.cluster_priority_compaction_factor_x = set_calibration(chosen_scenario, 16.0F, 20.0F, 50.0F);
   calibrations.cluster_priority_compaction_factor_y = set_calibration(chosen_scenario, 7.0F, 4.0F, 2.0F);
   calibrations.cluster_importance_decay_coef        = 0.1F;

   // get_unoccluded_dets_mask
   calibrations.init_azimuth_margin = 0.0F;
   calibrations.range_margin        = 5.2F;

   // initialize_contours
   calibrations.init_look_distance                         = 10.0F;
   calibrations.init_look_distance_underdrivable           = 18.0F;
   calibrations.min_init_segment_length                    = 0.5F;
   calibrations.sub_cluster_radius                         = 2.9F;
   calibrations.sensor_look_diversity_threshold            = 0.7F;
   calibrations.min_dets_span_for_stationary_hypothesis    = 30.0F;
   calibrations.init_vertex_covariance                     = 0.8F;
   calibrations.max_num_init_contoured_clusters            = 255U;
   calibrations.min_num_dets_for_stationary_hypothesis     = 80U;
   calibrations.min_num_vertices_for_stationary_hypothesis = 10U;
   calibrations.sub_min_cluster_points                     = 2U;
   calibrations.min_dets_for_contour_init                  = 2U;
   calibrations.min_dets_for_first_contour_init            = 2U;
   calibrations.max_det_age_for_contour_init               = 6U;
   ;
}

void CalibrationInitializerPlatform::initialize(ContourPostprocessingCalibs &calibrations, const Scenario &chosen_scenario) const
{
   calibrations.declutter_contours.contour_occlusion_threshold       = 0.5F;
   calibrations.declutter_contours.occluders_num_threshold           = 0U;
   calibrations.declutter_contours.contour_occlusion_azimuth_margin  = 0.0F;
   calibrations.declutter_contours.contour_occlusion_range_margin[0] = -0.5F;
   calibrations.declutter_contours.contour_occlusion_range_margin[1] = 6.0F;

   calibrations.merge_contours.merge_distance_threshold                      = set_calibration(chosen_scenario, 3.0F, 3.0F, 5.0F);
   calibrations.merge_contours.merge_max_angle                               = 60.0F;
   calibrations.merge_contours.merge_max_cumulative_angle                    = 210.0F;
   calibrations.merge_contours.merge_max_single_segment_length               = 0.5F;
   calibrations.merge_contours.max_merge_distance_to_ignore_cumulative_angle = 0.5F;
   calibrations.merge_contours.merge_longitudinal_squeeze_factor             = set_calibration(chosen_scenario, 0.8F, 0.8F, 0.5F);

   calibrations.simplify_contours.presimplify_host_length = 3.0F;
   calibrations.simplify_contours.presimplify_type        = 2U;
   calibrations.simplify_contours.presimplify_min_angle   = 30.0F;
   calibrations.simplify_contours.presimplify_max_angle   = 15.0F;

   calibrations.simplify_contours.simplify_min_turning_angle = 30.0F;
   calibrations.simplify_contours.simplify_type              = 1U;
   calibrations.simplify_contours.simplify_length_range[0]   = 3.0F;
   calibrations.simplify_contours.simplify_length_range[1]   = set_calibration(chosen_scenario, 10.0F, 10.0F, 20.0F);
   calibrations.simplify_contours.simplify_angle             = 30.0F;

   calibrations.remove_vertices.remove_vertices_front_limit = set_calibration(chosen_scenario, 120.0F, 120.0F, 220.0F);
   calibrations.remove_vertices.remove_vertices_rear_limit  = -10.0F;
   calibrations.remove_vertices.remove_vertices_left_limit  = -40.0F;
   calibrations.remove_vertices.remove_vertices_right_limit = 40.0F;
   calibrations.remove_vertices.accepted_uncertainty        = 0.81F;
   calibrations.remove_vertices.max_num_cycles_no_update    = 100U;

   calibrations.select_and_remove_excess_contours.max_num_contoured_clusters = 254U;
}

void CalibrationInitializerPlatform::initialize(SGDCFusionCalibs &calibrations, const Scenario &chosen_scenario) const
{
   (void)chosen_scenario; // MISRA
   calibrations.max_num_vertices_per_fused_contour = 100U;
   calibrations.max_num_fused_contours             = 100U;
   calibrations.max_num_fused_vertices             = 400U;
}

void CalibrationInitializerPlatform::initialize(DrivabilityClassificationCalibs &calibrations,
                                                const Scenario &chosen_scenario) const
{
   (void)chosen_scenario; // MISRA
   // critical region for drivability classification
   calibrations.create_subsegments                          = true;
   calibrations.critical_region_polygon[0]                  = {0.0F, -10.0F};
   calibrations.critical_region_polygon[1]                  = {180.0F, -20.0F};
   calibrations.critical_region_polygon[2]                  = {180.0F, 20.0F};
   calibrations.critical_region_polygon[3]                  = {0.0F, 10.0F};
   calibrations.critical_region_subdivisions                = 3U;
   calibrations.critical_region_min_abs_curvature_threshold = 0.0001F;
   calibrations.critical_region_max_abs_curvature_threshold = 0.02F;
   calibrations.subsegment_length                           = 1.5F;

   // update subsegments
   calibrations.projection_distance_threshold    = 0.2F;  // min distance to the nearest vertex when projection can take place
   calibrations.angle_between_segments_threshold = 0.26F; // min angle in radians between lines to consider them not parallel

   // assign_detections_to_subsegments_and_update_features
   calibrations.det_seg_assignment_threshold = 1.0F;

   // subsegments classification
   calibrations.min_num_dets_for_classification                 = 6.0F;
   calibrations.overdrivable_max_z                              = 0.2F;
   calibrations.underdrivable_min_z                             = 3.0F;
   calibrations.underdrivable_min_z_std                         = 4.0F;
   calibrations.decision_tree_obstacle_prob_thr                 = 0.5F;
   calibrations.decision_tree_overdrivability_obstacle_prob_thr = 0.5F;
   calibrations.classify_subsegments                            = true;

   // features calculation
   calibrations.features_inliers_threshold               = 0.25F;
   calibrations.features_radar_height                    = 0.0F;
   calibrations.features_z_max_threshold                 = 999.0F;
   calibrations.features_z_min_threshold                 = -999.0F;
   calibrations.features_min_rr_comp_of_stat_dealias_det = 5.0F;
   calibrations.features_max_azimuth_confidence          = 3;
   calibrations.features_max_elevation_confidence        = 3;
}

void CalibrationInitializerPlatform::initialize(ContourDownselectionCalibs &calibrations, const Scenario &chosen_scenario) const
{
   calibrations.min_number_of_valid_vertices                       = 2U;
   calibrations.base_priority_ratio                                = 0.7F;
   calibrations.already_selected_contours_priority_gain            = 0.3F;
   calibrations.curvi_roi_curvature_threshold                      = 0.001F;
   calibrations.roi_front_length_vcs                               = 80.0F;
   calibrations.roi_half_width_vcs                                 = 10.0F;
   calibrations.roi_rear_length_vcs                                = 20.0F;
   calibrations.contour_priority_shift_x                           = set_calibration(chosen_scenario, 0.0F, 5.0F, 40.0F);
   calibrations.contour_priority_compaction_factor_x               = set_calibration(chosen_scenario, 16.0F, 20.0F, 50.0F);
   calibrations.contour_priority_compaction_factor_y               = set_calibration(chosen_scenario, 7.0F, 4.0F, 2.0F);
   calibrations.min_x_to_downselect_contour                        = -10.0F;
   calibrations.min_required_underdrivable_share_to_reject_contour = 0.70F; // 70%
   calibrations.min_required_overdrivable_share_to_reject_contour  = 0.70F; // 70%
   calibrations.min_underdrivable_share_along_with_unclassified    = 0.40F; // 40%
   calibrations.min_unclassified_share_along_with_underdrivable    = 0.40F; // 40%
   calibrations.min_required_contour_priority_to_downselect        = 0.01F;
   calibrations.importance_decay_coef                              = 0.1F;
   calibrations.position_importance_saturation = 7.0F; // This is a highest squared x_span value, above which all
   calibrations.contour_length_sq_threshold    = 0.0F;
}
} // namespace sg
