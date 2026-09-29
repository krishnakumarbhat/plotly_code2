/*===========================================================================*\
* FILE: sg_calibrations.h
*============================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains calibrations used in Stationary Geometries.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef SG_CALIBRATIONS_H
#define SG_CALIBRATIONS_H

#include <array>
#include <utility>

#include "geometry/geo_point.h"
#include "sg_constants.h"
#include "sg_importance_calibrations.h"
#include "sg_reuse.h"

namespace sg
{
   struct Interval_T
   {
      float min{0.0F};
      float max{0.0F};
   };

   struct Vertical_T
   {
      Interval_T overground{0.0F, 0.0F};  // [m] overground view range [min, max]
      Interval_T underground{0.0F, 0.0F}; //[m] underground view range [min, max]
   };

   struct View_Range_T
   {
      Interval_T lateral{0.0F, 0.0F};      // [m] lateral view range [min, max]
      Interval_T longitudinal{0.0F, 0.0F}; // [m] longitudinal view range [min, max]
      Vertical_T vertical{};               // [m] vertical view range divided into overground and underground
   };

   struct Squeeze_Params_T
   {
      float factor{0.0F};
      float range{0.0F};
   };

   struct Time_Update_Calibrations_T
   {
      struct Process_Noise_T
      {
         float host_speed_weight_lat{0.01F};
         float host_speed_weight_lon{0.01F};
         float process_noise_base_std{0.01F};
         float host_speed_weight{0.01F};
      };

      Process_Noise_T noise{};
   };

   struct Downselect_Input_Detections_Calibrations_T
   {
      float max_rr_compens_for_stat_det{5.0F}; // [m/s] max range rate compensated for a stationary detection
      float accepted_probability_level{0.25F}; // [-] accepted probability level for a detection to be downselected [0-1]
      float max_host_speed_for_poor_azim_confid_det{2.0F}; // [m/s] maximum host speed to get detection with high azimuth
      float max_distance_for_poor_azim_confid_det{3.0F};   // [m] maximum detection distance with high azimuth
      int8_t maximum_valid_azimuth_confidence_value{2};    // [-] maximum valid azimuth confidence
   };

   struct R_Position_Covariance_T
   {
      float x{0.8F};
      float y{0.8F};
      float xy{0.0F};
   };

   struct View_Ranges_T
   {
      View_Range_T nondrivable{};
      View_Range_T overdrivable{};
      View_Range_T underdrivable{};

      View_Ranges_T()
      {
         nondrivable.lateral              = {-40.0F, 40.0F};
         nondrivable.longitudinal         = {-10.0F, 80.0F};
         nondrivable.vertical.underground = {-3.0F, -0.0F};
         nondrivable.vertical.overground  = {0.0F, 3.0F};

         overdrivable.lateral              = {-40.0F, 40.0F};
         overdrivable.longitudinal         = {-10.0F, 80.0F};
         overdrivable.vertical.underground = {-0.2F, -0.0F};
         overdrivable.vertical.overground  = {0.0F, 0.2F};

         underdrivable.lateral              = {-40.0F, 40.0F};
         underdrivable.longitudinal         = {-10.0F, 80.0F};
         underdrivable.vertical.underground = {-15.0F, -4.0F};
         underdrivable.vertical.overground  = {4.0F, 15.0F};
      }
   };

   struct Detection_Processing_Calibrations_T
   {
      Downselect_Input_Detections_Calibrations_T downselect_input_detections{};
      Importance_Calibrations_T importance_calibrations{};
   };

   // calibrations used by more than one processing step
   struct Common_Calibrations_T
   {
      R_Position_Covariance_T r_position_covariance{};
      View_Ranges_T view_ranges{};
      geometry::Point2D_T host_position{0.0F, 0.0F}; // [m] host position
      float min_segment_length{0.5F};
      float initial_reliability{3.0F};
      float azimuth_epsilon{1.0E-5F};
   };

   struct Linear_Piecewise_Transform_Coefficients_T
   {
      struct Coefficients_T
      {
         std::array<Squeeze_Params_T, 3> nondrivable;
         std::array<Squeeze_Params_T, 3> underdrivable;
      };

      Coefficients_T lateral;
      Coefficients_T longitudinal;

      Linear_Piecewise_Transform_Coefficients_T()
      {
         lateral.nondrivable[0U].range  = 10.0F;
         lateral.nondrivable[0U].factor = 1.0F;
         lateral.nondrivable[1U].range  = 15.0F;
         lateral.nondrivable[1U].factor = 1.0F;
         lateral.nondrivable[2U].range  = 20.0F;
         lateral.nondrivable[2U].factor = 1.0F;

         longitudinal.nondrivable[0U].range  = 30.0F;
         longitudinal.nondrivable[0U].factor = 1.0F;
         longitudinal.nondrivable[1U].range  = 60.0F;
         longitudinal.nondrivable[1U].factor = 1.0F;
         longitudinal.nondrivable[2U].range  = 100.0F;
         longitudinal.nondrivable[2U].factor = 0.8F;

         lateral.underdrivable[0U].range  = 66.0F;
         lateral.underdrivable[0U].factor = 0.4F;
         lateral.underdrivable[1U].range  = 120.0F;
         lateral.underdrivable[1U].factor = 0.2F;
         lateral.underdrivable[2U].range  = 200.0F;
         lateral.underdrivable[2U].factor = 0.1F;

         longitudinal.underdrivable[0U].range  = 66.0F;
         longitudinal.underdrivable[0U].factor = 0.6F;
         longitudinal.underdrivable[1U].range  = 170.0F;
         longitudinal.underdrivable[1U].factor = 0.4F;
         longitudinal.underdrivable[2U].range  = 200.0F;
         longitudinal.underdrivable[2U].factor = 0.4F;
      }
   };

   struct Existence_Probability_Calibrations_T
   {
      float neighbors_factor{0.9F};
      float age_factor{0.1F};
      float alpha{6.0F};
   };


   struct Cluster_Detections_Calibrations_T
   {
      Existence_Probability_Calibrations_T existence_probability{}; // [-] calibrations related to existence probability
      float cluster_radius_nondrivable{1.1F};                       // [m] cluster radius for nondrivable
      float cluster_radius_underdrivable{1.1F};                     // [m] cluster radius for underdrivable
      float historical_num_neighbors_forgetting_factor{0.9F};       // [-] forgetting factor for historical number of neighbors
      uint8_t min_cluster_points_nondrivable{4U};   // [-] minimum amount of neighbors in region to create expand cluster [for
                                                    // nondrivable]
      uint8_t min_cluster_points_underdrivable{2U}; // [-] minimum amount of neighbors in region to create expand cluster [for
                                                    // underdrivable]
   };

   struct Detection_Clustering_Calibrations_T
   {
      Linear_Piecewise_Transform_Coefficients_T linear_piecewise_transform_coefficients;
      Cluster_Detections_Calibrations_T cluster_detections;
   };

   struct Measurement_Association_Calibrations_T
   {
      // associate_detections_to_contour
      float width_min{1.8F};
      float width_max{3.0F};
      float length_margin_min{1.0F};
      float length_margin_max{1.6F};
      float length_margin_ending{1.0F};
      bool f_dynamic_gates{true};
      uint16_t min_dets_assoc_update_cluster_id{1U};

      // associate_cluster_boundary_detections_to_contour
      float width_extension_min{1.3F};
      float width_extension_max{2.5F};
      float extended_meas_covariance_xx{0.1F};
      float extended_meas_covariance_yy{0.1F};

      float dynamic_gate_lower_distance{40.0F};
      float dynamic_gate_upper_distance{100.0F};
   };

   struct Measurement_Update_Calibrations_T
   {
      float length_measurement_noise{0.8F};
      float length_forgetting_factor{0.98F};
      float minimal_length_spread{0.1F};
      float position_forgetting_factor{0.995F};
      float regularity_weight{0.8F}; // (1/regularity variance), must be balanced with R_position_covariance_x, higher value = more
                                     // regularization
      float regularity_threshold{0.1F}; // squared distance between 3 vertex subcontour endvertices average and second vertex,
                                        // higher value = regularization terminates earlier
      uint8_t min_dets_for_shrinking{1U};
      uint8_t max_num_ending_segment_dets{static_cast<uint8_t>(SG_MAX_NUM_ENDING_SEGMENT_DETS)};
   };

   struct Contour_Initialization_Calibrations_T
   {
      // get_unoccluded_dets_mask
      float init_azimuth_margin{0.0F};
      float range_margin{5.2F};

      // initialize_contours
      float init_look_distance{10.0F};
      float init_look_distance_underdrivable{18.0F};
      float min_init_segment_length{0.5F};
      float sub_cluster_radius{2.9F};
      float sensor_look_diversity_threshold{0.7F};
      float min_dets_span_for_stationary_hypothesis{30.0F};
      float init_vertex_covariance{0.8F};
      uint16_t max_num_init_contoured_clusters{255U};
      uint16_t min_num_dets_for_stationary_hypothesis{80U};
      uint16_t min_num_vertices_for_stationary_hypothesis{10U};
      uint8_t sub_min_cluster_points{2U};
      uint8_t min_dets_for_contour_init{2U};
      uint8_t min_dets_for_first_contour_init{2U};
   };

   struct Contour_Postprocessing_Calibrations_T
   {
      struct Declutter_Contours_T
      {
         // get_occluded_contours
         float contour_occlusion_threshold{0.5F};
         uint8_t occluders_num_threshold{0U};
         // get_occluded_vertices
         float contour_occlusion_azimuth_margin{0.0F};
         float contour_occlusion_range_margin[2]{-0.5F, 6.0F};
      };

      struct Merge_Contours_T
      {
         // merge_contours
         // description for merge contours calibrations:
         // https://confluence.asux.aptiv.com/pages/viewpage.action?pageId=705929486
         float merge_distance_threshold{3.0F};
         float merge_max_angle{60.0F};
         float merge_max_cumulative_angle{210.0F};
         float merge_max_single_segment_length{0.5F};               // for single segment contours whose length is smaller than
                                                                    // this threshold, the cumulative angle is ignored
         float max_merge_distance_to_ignore_cumulative_angle{0.5F}; // if vertex distance between merge partners is smaller than
                                                                    // this threshold, cumulative angle requirements are not
                                                                    // necessary
         float merge_longitudinal_squeeze_factor{0.8F};
      };

      struct Simplify_Contours_T
      {
         // presimplify_contours
         float presimplify_host_length{3.0F};
         uint8_t presimplify_type{2U};
         float presimplify_min_angle{30.0F};
         float presimplify_max_angle{15.0F};

         // simplify_contours
         float simplify_min_turning_angle{30.0F};
         uint8_t simplify_type{1U};
         float simplify_length_range[2]{3.0F, 10.0F};
         float simplify_angle{30.0F};
      };

      struct Remove_Vertices_T
      {
         // remove_vertices
         float remove_vertices_front_limit{120.0F};
         float remove_vertices_rear_limit{-10.0F};
         float remove_vertices_left_limit{-40.0F};
         float remove_vertices_right_limit{40.0F};
         float accepted_uncertainty{0.81F};
         uint16_t max_num_cycles_no_update{100U};
      };

      struct Select_And_Remove_Excess_Contours_T
      {
         uint16_t max_num_contoured_clusters{254U};
      };

      Declutter_Contours_T declutter_contours{};
      Merge_Contours_T merge_contours{};
      Simplify_Contours_T simplify_contours{};
      Remove_Vertices_T remove_vertices{};
      Select_And_Remove_Excess_Contours_T select_and_remove_excess_contours{};
   };

   struct SG_DC_Fusion_Calibrations_T
   {
      uint16_t max_num_vertices_per_fused_contour{100U};
      uint16_t max_num_fused_contours{100U};
      uint16_t max_num_fused_vertices{400U};
   };

   struct Drivability_Classification_Calibrations_T
   {
      // critical region for drivability classification
      bool create_subsegments{true};
      geometry::Point2D_T critical_region_polygon[DC_NUM_POLYGON_VERTICES_BEFORE_INTERPOLATION]{
         {0.0F, -10.0F}, {180.0F, -20.0F}, {180.0F, 20.0F}, {0.0F, 10.0F}};
      uint8_t critical_region_subdivisions{3U};
      float critical_region_min_abs_curvature_threshold{0.0001F};
      float critical_region_max_abs_curvature_threshold{0.02F};
      float subsegment_length{1.5F};

      // update subsegments
      float projection_distance_threshold{0.2F};     // min distance to the nearest vertex when projection can take place
      float angle_between_segments_threshold{0.26F}; // min angle in radians between lines to consider them not parallel

      // assign_detections_to_subsegments_and_update_features
      float det_seg_assignment_threshold{1.0F};

      // subsegments classification
      float min_num_dets_for_classification{6.0F};
      float overdrivable_max_z{0.2F};
      float underdrivable_min_z{3.0F};
      float underdrivable_min_z_std{4.0F};
      float decision_tree_obstacle_prob_thr{0.5F};
      float decision_tree_overdrivability_obstacle_prob_thr{0.5F};
      bool classify_subsegments{true};

      // features calculation
      float features_inliers_threshold{0.25F};
      float features_radar_height{0.0F};
      float features_z_max_threshold{999.0F};
      float features_z_min_threshold{-999.0F};
      float features_min_rr_comp_of_stat_dealias_det{5.0F};
      int8_t features_max_azimuth_confidence{3};
      int8_t features_max_elevation_confidence{3};
   };

   struct Contour_Downselection_Calibrations_T
   {
      uint16_t min_number_of_valid_vertices{2U};
      float curvi_roi_curvature_threshold{0.001F};
      float roi_front_length_vcs{80.0F};
      float roi_half_width_vcs{10.0F};
      float roi_rear_length_vcs{20.0F};
      float contour_priority_shift_x{0.0F};
      float contour_priority_compaction_factor_x{1.0F};
      float contour_priority_compaction_factor_y{1.0F};
      float min_x_to_downselect_contour{-10.0F};
      float min_required_underdrivable_share_to_reject_contour{0.70F}; // 70%
      float min_required_overdrivable_share_to_reject_contour{0.70F};  // 70%
      float min_underdrivable_share_along_with_unclassified{0.40F};    // 40%
      float min_unclassified_share_along_with_underdrivable{0.40F};    // 40%
      float min_required_contour_priority_to_downselect{0.01F};
      float importance_decay_coef{0.1F};
      float position_importance_saturation{7.0F}; // This is a highest squared x_span value, above which all
                                                  // longer contours are given the same maximum priority.
   };

   struct Calibrations_T
   {
      uint64_t elapsed_time_th{50000U}; // [us] Expected time elapsed between two consecutive scans
      Common_Calibrations_T common{};
      Time_Update_Calibrations_T time_update{};
      Detection_Processing_Calibrations_T detection_processing{};
      Detection_Clustering_Calibrations_T detection_clustering{};
      Measurement_Association_Calibrations_T measurement_association{};
      Measurement_Update_Calibrations_T measurement_update{};
      Contour_Initialization_Calibrations_T contour_initialization{};
      Contour_Postprocessing_Calibrations_T contour_postprocessing{};
      Drivability_Classification_Calibrations_T drivability_classification{};
      SG_DC_Fusion_Calibrations_T sg_dc_fusion{};
      Contour_Downselection_Calibrations_T contour_downselection{};
   };
}
#endif
