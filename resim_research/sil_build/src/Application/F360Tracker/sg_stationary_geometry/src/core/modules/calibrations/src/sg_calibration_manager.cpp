#include "sg_calibration_manager.h"

namespace sg
{
   constexpr float CalibrationManager::m_velocity_thresholds[];

   CalibrationManager::CalibrationManager()
   {
      static_assert((m_velocity_thresholds[static_cast<uint8_t>(CalibrationIndices::INDEX_CITY)]
                     - m_velocity_thresholds[static_cast<uint8_t>(CalibrationIndices::INDEX_PARKING)])
                       > std::numeric_limits<float>::epsilon(),
                    "Velocity difference between city and parking scenario should be greater than epsilon");
      static_assert((m_velocity_thresholds[static_cast<uint8_t>(CalibrationIndices::INDEX_HIGHWAY)]
                     - m_velocity_thresholds[static_cast<uint8_t>(CalibrationIndices::INDEX_CITY)])
                       > std::numeric_limits<float>::epsilon(),
                    "Velocity difference between city and parking scenario should be greater than epsilon");
      static_assert(m_velocity_thresholds[static_cast<uint8_t>(CalibrationIndices::INDEX_PARKING)]
                       < m_velocity_thresholds[static_cast<uint8_t>(CalibrationIndices::INDEX_CITY)],
                    "Parking velocity threshold should be less then city velocity threshold");
      static_assert(m_velocity_thresholds[static_cast<uint8_t>(CalibrationIndices::INDEX_CITY)]
                       < m_velocity_thresholds[static_cast<uint8_t>(CalibrationIndices::INDEX_HIGHWAY)],
                    "City velocity threshold should be less then highway velocity threshold");
      constexpr auto f_valid_num_sets = (static_cast<uint8_t>(CalibrationIndices::NUMBER_OF_CALIBRATION_SETS) == 3U);
      static_assert(f_valid_num_sets, "If you want to add/remove a new calibration index, you must update both: calibration LUT "
                                      "and velocity threshold arrays along with CalibrationManager::set_calibration_indices "
                                      "method");

      initialize_calibration_intervals();
      m_resulting_calibration = m_calibration_lut[static_cast<std::uint8_t>(CalibrationIndices::INDEX_PARKING)];
   }

   void CalibrationManager::set_calibration_indices(CalibrationIndices &index_low,
                                                    CalibrationIndices &index_high,
                                                    const float calibration_arg) const
   {
      if (calibration_arg <= m_velocity_thresholds[static_cast<std::uint8_t>(CalibrationIndices::INDEX_PARKING)])
      {
         index_low  = CalibrationIndices::INDEX_PARKING;
         index_high = CalibrationIndices::INDEX_PARKING;
      }
      else if (calibration_arg <= m_velocity_thresholds[static_cast<std::uint8_t>(CalibrationIndices::INDEX_CITY)])
      {
         index_low  = CalibrationIndices::INDEX_PARKING;
         index_high = CalibrationIndices::INDEX_CITY;
      }
      else if (calibration_arg <= m_velocity_thresholds[static_cast<std::uint8_t>(CalibrationIndices::INDEX_HIGHWAY)])
      {
         index_low  = CalibrationIndices::INDEX_CITY;
         index_high = CalibrationIndices::INDEX_HIGHWAY;
      }
      else // m_velocity_thresholds[CalibrationIndices::INDEX_HIGHWAY] < calibration_arg
      {
         index_low  = CalibrationIndices::INDEX_HIGHWAY;
         index_high = CalibrationIndices::INDEX_HIGHWAY;
      }
   }

   void CalibrationManager::update(const float host_speed)
   {
      CalibrationIndices index_low;
      CalibrationIndices index_high;
      set_calibration_indices(index_low, index_high, host_speed);
      assign_current_calibration(index_low, index_high, host_speed);
   }

   void CalibrationManager::reset()
   {
      m_resulting_calibration = m_calibration_lut[static_cast<std::uint8_t>(CalibrationIndices::INDEX_PARKING)];
   }

   Calibrations_T &CalibrationManager::get()
   {
      return m_resulting_calibration;
   }

   void CalibrationManager::initialize_calibration_intervals()
   {
      /* TODO FZD-958:
         Do an extra initialization of calibrations for specific intervals, handled at the indices:
         CalibrationIndices::INDEX_PARKING
         CalibrationIndices::INDEX_CITY
         CalibrationIndices::INDEX_HIGHWAY
         Update following function:
      */
      initialize_calibration_interval_for_parking();
      initialize_calibration_interval_for_city();
      initialize_calibration_interval_for_highway();
   }

   void CalibrationManager::initialize_calibration_interval_for_parking()
   {
      /* TODO FZD-958 - keep updated this extra initialization */
      Calibrations_T &parking_calibrations = m_calibration_lut[static_cast<std::uint8_t>(CalibrationIndices::INDEX_PARKING)];

      // downselect_input_detections
      parking_calibrations.detection_processing.downselect_input_detections.max_rr_compens_for_stat_det = 5.0F;

      // contour downselection
      parking_calibrations.contour_downselection.contour_priority_shift_x             = 0.0F;
      parking_calibrations.contour_downselection.contour_priority_compaction_factor_x = 16.0F;
      parking_calibrations.contour_downselection.contour_priority_compaction_factor_y = 7.0F;
   }

   void CalibrationManager::initialize_calibration_interval_for_city()
   {
      /* TODO FZD-958 - keep updated based on realdata_city_dataset_cfg.m */
      Calibrations_T &city_calibrations = m_calibration_lut[static_cast<std::uint8_t>(CalibrationIndices::INDEX_CITY)];

      // contour downselection
      city_calibrations.contour_downselection.contour_priority_shift_x             = 5.0F;
      city_calibrations.contour_downselection.contour_priority_compaction_factor_x = 20.0F;
      city_calibrations.contour_downselection.contour_priority_compaction_factor_y = 4.0F;
   }

   void CalibrationManager::initialize_calibration_interval_for_highway()
   {
      /* TODO FZD-958 - keep updated based on realdata_golden_dataset_cfg.m */
      Calibrations_T &highway_calibrations = m_calibration_lut[static_cast<std::uint8_t>(CalibrationIndices::INDEX_HIGHWAY)];

      // association
      highway_calibrations.measurement_association.length_margin_ending = 1.5F;

      // downselection
      highway_calibrations.common.view_ranges.nondrivable.longitudinal.max   = 180.0F;
      highway_calibrations.common.view_ranges.underdrivable.longitudinal.max = 180.0F;
      highway_calibrations.common.view_ranges.overdrivable.longitudinal.max  = 180.0F;

      // remove vertices
      highway_calibrations.contour_postprocessing.remove_vertices.remove_vertices_front_limit = 220.0F;

      // merging
      highway_calibrations.contour_postprocessing.merge_contours.merge_distance_threshold          = 5.0F;
      highway_calibrations.contour_postprocessing.merge_contours.merge_longitudinal_squeeze_factor = 0.5F;

      // simplification
      highway_calibrations.contour_postprocessing.simplify_contours.simplify_length_range[1] = 20.0F;

      highway_calibrations.detection_clustering.linear_piecewise_transform_coefficients.longitudinal.nondrivable[0].range  = 66.0F;
      highway_calibrations.detection_clustering.linear_piecewise_transform_coefficients.longitudinal.nondrivable[0].factor = 0.3F;
      highway_calibrations.detection_clustering.linear_piecewise_transform_coefficients.longitudinal.nondrivable[1].range = 170.0F;
      highway_calibrations.detection_clustering.linear_piecewise_transform_coefficients.longitudinal.nondrivable[1].factor = 0.2F;
      highway_calibrations.detection_clustering.linear_piecewise_transform_coefficients.longitudinal.nondrivable[2].range = 200.0F;
      highway_calibrations.detection_clustering.linear_piecewise_transform_coefficients.longitudinal.nondrivable[2].factor = 0.1F;

      // contour downselection
      highway_calibrations.contour_downselection.contour_priority_shift_x             = 40.0F;
      highway_calibrations.contour_downselection.contour_priority_compaction_factor_x = 50.0F;
      highway_calibrations.contour_downselection.contour_priority_compaction_factor_y = 2.0F;

      // measurement update
      highway_calibrations.measurement_update.regularity_weight = 5.0F;
   }

   void CalibrationManager::interpolate_calibrations(const Interpolator &the_interpolator,
                                                     const Common_Calibrations_T &low,
                                                     const Common_Calibrations_T &high)
   {
      /* Always update when adding a new field to the Common_Calibrations_T structure. */
      static_assert((sizeof(Common_Calibrations_T) == 128U), "struct Common_Calibrations_T has changed, update "
                                                             "interpolate_calibrations to interpolate and check new signal(s).");

      auto &result = m_resulting_calibration.common;

      // combine_detections
      result.r_position_covariance.x  = the_interpolator.interpolate(low.r_position_covariance.x, high.r_position_covariance.x);
      result.r_position_covariance.y  = the_interpolator.interpolate(low.r_position_covariance.y, high.r_position_covariance.y);
      result.r_position_covariance.xy = the_interpolator.interpolate(low.r_position_covariance.xy, high.r_position_covariance.xy);

      // common - view ranges
      result.view_ranges.nondrivable.lateral.min =
         the_interpolator.interpolate(low.view_ranges.nondrivable.lateral.min, high.view_ranges.nondrivable.lateral.min);
      result.view_ranges.nondrivable.lateral.max =
         the_interpolator.interpolate(low.view_ranges.nondrivable.lateral.max, high.view_ranges.nondrivable.lateral.max);
      result.view_ranges.nondrivable.longitudinal.min =
         the_interpolator.interpolate(low.view_ranges.nondrivable.longitudinal.min, high.view_ranges.nondrivable.longitudinal.min);
      result.view_ranges.nondrivable.longitudinal.max =
         the_interpolator.interpolate(low.view_ranges.nondrivable.longitudinal.max, high.view_ranges.nondrivable.longitudinal.max);
      result.view_ranges.nondrivable.vertical.overground.min = the_interpolator.interpolate(
         low.view_ranges.nondrivable.vertical.overground.min, high.view_ranges.nondrivable.vertical.overground.min);
      result.view_ranges.nondrivable.vertical.overground.max = the_interpolator.interpolate(
         low.view_ranges.nondrivable.vertical.overground.max, high.view_ranges.nondrivable.vertical.overground.max);
      result.view_ranges.nondrivable.vertical.underground.min = the_interpolator.interpolate(
         low.view_ranges.nondrivable.vertical.underground.min, high.view_ranges.nondrivable.vertical.underground.min);
      result.view_ranges.nondrivable.vertical.underground.max = the_interpolator.interpolate(
         low.view_ranges.nondrivable.vertical.underground.max, high.view_ranges.nondrivable.vertical.underground.max);
      result.view_ranges.overdrivable.lateral.min =
         the_interpolator.interpolate(low.view_ranges.overdrivable.lateral.min, high.view_ranges.overdrivable.lateral.min);
      result.view_ranges.overdrivable.lateral.max =
         the_interpolator.interpolate(low.view_ranges.overdrivable.lateral.max, high.view_ranges.overdrivable.lateral.max);
      result.view_ranges.overdrivable.longitudinal.min =
         the_interpolator.interpolate(low.view_ranges.overdrivable.longitudinal.min, high.view_ranges.overdrivable.longitudinal.min);
      result.view_ranges.overdrivable.longitudinal.max =
         the_interpolator.interpolate(low.view_ranges.overdrivable.longitudinal.max, high.view_ranges.overdrivable.longitudinal.max);
      result.view_ranges.overdrivable.vertical.overground.min = the_interpolator.interpolate(
         low.view_ranges.overdrivable.vertical.overground.min, high.view_ranges.overdrivable.vertical.overground.min);
      result.view_ranges.overdrivable.vertical.overground.max = the_interpolator.interpolate(
         low.view_ranges.overdrivable.vertical.overground.max, high.view_ranges.overdrivable.vertical.overground.max);
      result.view_ranges.overdrivable.vertical.underground.min = the_interpolator.interpolate(
         low.view_ranges.overdrivable.vertical.underground.min, high.view_ranges.overdrivable.vertical.underground.min);
      result.view_ranges.overdrivable.vertical.underground.max = the_interpolator.interpolate(
         low.view_ranges.overdrivable.vertical.underground.max, high.view_ranges.overdrivable.vertical.underground.max);
      result.view_ranges.underdrivable.lateral.min =
         the_interpolator.interpolate(low.view_ranges.underdrivable.lateral.min, high.view_ranges.underdrivable.lateral.min);
      result.view_ranges.underdrivable.lateral.max =
         the_interpolator.interpolate(low.view_ranges.underdrivable.lateral.max, high.view_ranges.underdrivable.lateral.max);
      result.view_ranges.underdrivable.longitudinal.min = the_interpolator.interpolate(
         low.view_ranges.underdrivable.longitudinal.min, high.view_ranges.underdrivable.longitudinal.min);
      result.view_ranges.underdrivable.longitudinal.max = the_interpolator.interpolate(
         low.view_ranges.underdrivable.longitudinal.max, high.view_ranges.underdrivable.longitudinal.max);
      result.view_ranges.underdrivable.vertical.overground.min = the_interpolator.interpolate(
         low.view_ranges.underdrivable.vertical.overground.min, high.view_ranges.underdrivable.vertical.overground.min);
      result.view_ranges.underdrivable.vertical.overground.max = the_interpolator.interpolate(
         low.view_ranges.underdrivable.vertical.overground.max, high.view_ranges.underdrivable.vertical.overground.max);
      result.view_ranges.underdrivable.vertical.underground.min = the_interpolator.interpolate(
         low.view_ranges.underdrivable.vertical.underground.min, high.view_ranges.underdrivable.vertical.underground.min);
      result.view_ranges.underdrivable.vertical.underground.max = the_interpolator.interpolate(
         low.view_ranges.underdrivable.vertical.underground.max, high.view_ranges.underdrivable.vertical.underground.max);
      result.min_segment_length  = the_interpolator.interpolate(low.min_segment_length, high.min_segment_length);
      result.initial_reliability = the_interpolator.interpolate(low.initial_reliability, high.initial_reliability);
      result.azimuth_epsilon     = the_interpolator.interpolate(low.azimuth_epsilon, high.azimuth_epsilon);
   }


   void CalibrationManager::interpolate_calibrations(const Interpolator &the_interpolator,
                                                     const Detection_Processing_Calibrations_T &low,
                                                     const Detection_Processing_Calibrations_T &high)
   {
      /* Always update when adding a new field to the Detection_Processing_Calibrations_T structure. */
      static_assert((sizeof(Detection_Processing_Calibrations_T) == 56U), "struct Detection_Processing_Calibrations_T has "
                                                                          "changed, update interpolate_calibrations to "
                                                                          "interpolate and check new signal(s).");

      auto &result = m_resulting_calibration.detection_processing;

      // downselect_input_detections
      result.downselect_input_detections.max_rr_compens_for_stat_det = the_interpolator.interpolate(
         low.downselect_input_detections.max_rr_compens_for_stat_det, high.downselect_input_detections.max_rr_compens_for_stat_det);

      // downselect input detections
      result.downselect_input_detections.accepted_probability_level = the_interpolator.interpolate(
         low.downselect_input_detections.accepted_probability_level, high.downselect_input_detections.accepted_probability_level);
      result.downselect_input_detections.maximum_valid_azimuth_confidence_value =
         the_interpolator.interpolate(low.downselect_input_detections.maximum_valid_azimuth_confidence_value,
                                      high.downselect_input_detections.maximum_valid_azimuth_confidence_value);
   }

   void CalibrationManager::interpolate_calibrations(const Interpolator &the_interpolator,
                                                     const Detection_Clustering_Calibrations_T &low,
                                                     const Detection_Clustering_Calibrations_T &high)
   {
      /* Always update when adding a new field to the Detection_Clustering_Calibrations_T structure. */
      static_assert((sizeof(Detection_Clustering_Calibrations_T) == 124U), "struct Detection_Clustering_Calibrations_T has "
                                                                           "changed, update interpolate_calibrations to "
                                                                           "interpolate and check new signal(s).");


      auto &result           = m_resulting_calibration.detection_clustering.linear_piecewise_transform_coefficients;
      const auto &low_calib  = low.linear_piecewise_transform_coefficients;
      const auto &high_calib = high.linear_piecewise_transform_coefficients;

      result.lateral.nondrivable[0].range =
         the_interpolator.interpolate(low_calib.lateral.nondrivable[0].range, high_calib.lateral.nondrivable[0].range);
      result.lateral.nondrivable[0].factor =
         the_interpolator.interpolate(low_calib.lateral.nondrivable[0].factor, high_calib.lateral.nondrivable[0].factor);
      result.lateral.nondrivable[1].range =
         the_interpolator.interpolate(low_calib.lateral.nondrivable[1].range, high_calib.lateral.nondrivable[1].range);
      result.lateral.nondrivable[1].factor =
         the_interpolator.interpolate(low_calib.lateral.nondrivable[1].factor, high_calib.lateral.nondrivable[1].factor);
      result.lateral.nondrivable[2].range =
         the_interpolator.interpolate(low_calib.lateral.nondrivable[2].range, high_calib.lateral.nondrivable[2].range);
      result.lateral.nondrivable[2].factor =
         the_interpolator.interpolate(low_calib.lateral.nondrivable[2].factor, high_calib.lateral.nondrivable[2].factor);

      result.longitudinal.nondrivable[0].range =
         the_interpolator.interpolate(low_calib.longitudinal.nondrivable[0].range, high_calib.longitudinal.nondrivable[0].range);
      result.longitudinal.nondrivable[0].factor =
         the_interpolator.interpolate(low_calib.longitudinal.nondrivable[0].factor, high_calib.longitudinal.nondrivable[0].factor);
      result.longitudinal.nondrivable[1].range =
         the_interpolator.interpolate(low_calib.longitudinal.nondrivable[1].range, high_calib.longitudinal.nondrivable[1].range);
      result.longitudinal.nondrivable[1].factor =
         the_interpolator.interpolate(low_calib.longitudinal.nondrivable[1].factor, high_calib.longitudinal.nondrivable[1].factor);
      result.longitudinal.nondrivable[2].range =
         the_interpolator.interpolate(low_calib.longitudinal.nondrivable[2].range, high_calib.longitudinal.nondrivable[2].range);
      result.longitudinal.nondrivable[2].factor =
         the_interpolator.interpolate(low_calib.longitudinal.nondrivable[2].factor, high_calib.longitudinal.nondrivable[2].factor);

      result.lateral.underdrivable[0].range =
         the_interpolator.interpolate(low_calib.lateral.underdrivable[0].range, high_calib.lateral.underdrivable[0].range);
      result.lateral.underdrivable[0].factor =
         the_interpolator.interpolate(low_calib.lateral.underdrivable[0].factor, high_calib.lateral.underdrivable[0].factor);
      result.lateral.underdrivable[1].range =
         the_interpolator.interpolate(low_calib.lateral.underdrivable[1].range, high_calib.lateral.underdrivable[1].range);
      result.lateral.underdrivable[1].factor =
         the_interpolator.interpolate(low_calib.lateral.underdrivable[1].factor, high_calib.lateral.underdrivable[1].factor);
      result.lateral.underdrivable[2].range =
         the_interpolator.interpolate(low_calib.lateral.underdrivable[2].range, high_calib.lateral.underdrivable[2].range);
      result.lateral.underdrivable[2].factor =
         the_interpolator.interpolate(low_calib.lateral.underdrivable[2].factor, high_calib.lateral.underdrivable[2].factor);

      result.longitudinal.underdrivable[0].range  = the_interpolator.interpolate(low_calib.longitudinal.underdrivable[0].range,
                                                                                 high_calib.longitudinal.underdrivable[0].range);
      result.longitudinal.underdrivable[0].factor = the_interpolator.interpolate(low_calib.longitudinal.underdrivable[0].factor,
                                                                                 high_calib.longitudinal.underdrivable[0].factor);
      result.longitudinal.underdrivable[1].range  = the_interpolator.interpolate(low_calib.longitudinal.underdrivable[1].range,
                                                                                 high_calib.longitudinal.underdrivable[1].range);
      result.longitudinal.underdrivable[1].factor = the_interpolator.interpolate(low_calib.longitudinal.underdrivable[1].factor,
                                                                                 high_calib.longitudinal.underdrivable[1].factor);
      result.longitudinal.underdrivable[2].range  = the_interpolator.interpolate(low_calib.longitudinal.underdrivable[2].range,
                                                                                 high_calib.longitudinal.underdrivable[2].range);
      result.longitudinal.underdrivable[2].factor = the_interpolator.interpolate(low_calib.longitudinal.underdrivable[2].factor,
                                                                                 high_calib.longitudinal.underdrivable[2].factor);

      auto &result_cluster_detections = m_resulting_calibration.detection_clustering.cluster_detections;

      // cluster_detections
      result_cluster_detections.cluster_radius_nondrivable = the_interpolator.interpolate(
         low.cluster_detections.cluster_radius_nondrivable, high.cluster_detections.cluster_radius_nondrivable);
      result_cluster_detections.cluster_radius_underdrivable = the_interpolator.interpolate(
         low.cluster_detections.cluster_radius_underdrivable, high.cluster_detections.cluster_radius_underdrivable);
      result_cluster_detections.min_cluster_points_nondrivable = the_interpolator.interpolate(
         low.cluster_detections.min_cluster_points_nondrivable, high.cluster_detections.min_cluster_points_nondrivable);
      result_cluster_detections.min_cluster_points_underdrivable = the_interpolator.interpolate(
         low.cluster_detections.min_cluster_points_underdrivable, high.cluster_detections.min_cluster_points_underdrivable);

      result_cluster_detections.historical_num_neighbors_forgetting_factor =
         the_interpolator.interpolate(low.cluster_detections.historical_num_neighbors_forgetting_factor,
                                      high.cluster_detections.historical_num_neighbors_forgetting_factor);

      // compute_existence_probability
      result_cluster_detections.existence_probability.neighbors_factor =
         the_interpolator.interpolate(low.cluster_detections.existence_probability.neighbors_factor,
                                      high.cluster_detections.existence_probability.neighbors_factor);
      result_cluster_detections.existence_probability.age_factor = the_interpolator.interpolate(
         low.cluster_detections.existence_probability.age_factor, high.cluster_detections.existence_probability.age_factor);
      result_cluster_detections.existence_probability.alpha = the_interpolator.interpolate(
         low.cluster_detections.existence_probability.alpha, high.cluster_detections.existence_probability.alpha);
   }

   void CalibrationManager::interpolate_calibrations(const Interpolator &the_interpolator,
                                                     const Measurement_Association_Calibrations_T &low,
                                                     const Measurement_Association_Calibrations_T &high)
   {
      auto &result = m_resulting_calibration.measurement_association;

      // associate_detections_to_contour
      result.width_min            = the_interpolator.interpolate(low.width_min, high.width_min);
      result.length_margin_min    = the_interpolator.interpolate(low.length_margin_min, high.length_margin_min);
      result.width_max            = the_interpolator.interpolate(low.width_max, high.width_max);
      result.length_margin_max    = the_interpolator.interpolate(low.length_margin_max, high.length_margin_max);
      result.length_margin_ending = the_interpolator.interpolate(low.length_margin_ending, high.length_margin_ending);
      result.min_dets_assoc_update_cluster_id =
         the_interpolator.interpolate(low.min_dets_assoc_update_cluster_id, high.min_dets_assoc_update_cluster_id);
      result.extended_meas_covariance_xx =
         the_interpolator.interpolate(low.extended_meas_covariance_xx, high.extended_meas_covariance_xx);
      result.extended_meas_covariance_yy =
         the_interpolator.interpolate(low.extended_meas_covariance_yy, high.extended_meas_covariance_yy);

      // associate_cluster_boundary_detections_to_contour
      result.width_extension_min = the_interpolator.interpolate(low.width_extension_min, high.width_extension_min);
      result.width_extension_max = the_interpolator.interpolate(low.width_extension_max, high.width_extension_max);
      result.extended_meas_covariance_xx =
         the_interpolator.interpolate(low.extended_meas_covariance_xx, high.extended_meas_covariance_xx);
      result.extended_meas_covariance_yy =
         the_interpolator.interpolate(low.extended_meas_covariance_yy, high.extended_meas_covariance_yy);
   }

   void CalibrationManager::interpolate_calibrations(const Interpolator &the_interpolator,
                                                     const Measurement_Update_Calibrations_T &low,
                                                     const Measurement_Update_Calibrations_T &high)
   {
      auto &result = m_resulting_calibration.measurement_update;

      result.length_measurement_noise = the_interpolator.interpolate(low.length_measurement_noise, high.length_measurement_noise);
      result.position_forgetting_factor =
         the_interpolator.interpolate(low.position_forgetting_factor, high.position_forgetting_factor);
      result.length_forgetting_factor = the_interpolator.interpolate(low.length_forgetting_factor, high.length_forgetting_factor);
      result.minimal_length_spread    = the_interpolator.interpolate(low.minimal_length_spread, high.minimal_length_spread);
      result.regularity_weight        = the_interpolator.interpolate(low.regularity_weight, high.regularity_weight);
      result.regularity_threshold     = the_interpolator.interpolate(low.regularity_threshold, high.regularity_threshold);
      result.min_dets_for_shrinking   = the_interpolator.interpolate(low.min_dets_for_shrinking, high.min_dets_for_shrinking);
      result.max_num_ending_segment_dets =
         the_interpolator.interpolate(low.max_num_ending_segment_dets, high.max_num_ending_segment_dets);
   }

   void CalibrationManager::interpolate_calibrations(const Interpolator &the_interpolator,
                                                     const Contour_Initialization_Calibrations_T &low,
                                                     const Contour_Initialization_Calibrations_T &high)
   {
      /* Always update when adding a new field to the Contour_Initialization_Calibrations_T structure. */
      static_assert((sizeof(Contour_Initialization_Calibrations_T) == 48U), "struct Contour_Initialization_Calibrations_T has "
                                                                            "changed, update interpolate_calibrations to "
                                                                            "interpolate and check new signal(s).");

      auto &result = m_resulting_calibration.contour_initialization;

      // initialize_contours
      result.init_look_distance = the_interpolator.interpolate(low.init_look_distance, high.init_look_distance);
      result.init_look_distance_underdrivable =
         the_interpolator.interpolate(low.init_look_distance_underdrivable, high.init_look_distance_underdrivable);
      result.min_init_segment_length = the_interpolator.interpolate(low.min_init_segment_length, high.min_init_segment_length);
      result.sub_cluster_radius      = the_interpolator.interpolate(low.sub_cluster_radius, high.sub_cluster_radius);
      result.sub_min_cluster_points  = the_interpolator.interpolate(low.sub_min_cluster_points, high.sub_min_cluster_points);
      result.min_dets_for_contour_init = the_interpolator.interpolate(low.min_dets_for_contour_init, high.min_dets_for_contour_init);
      result.min_dets_for_first_contour_init =
         the_interpolator.interpolate(low.min_dets_for_first_contour_init, high.min_dets_for_first_contour_init);
      result.max_num_init_contoured_clusters =
         the_interpolator.interpolate(low.max_num_init_contoured_clusters, high.max_num_init_contoured_clusters);
      result.init_vertex_covariance = the_interpolator.interpolate(low.init_vertex_covariance, high.init_vertex_covariance);

      // get_unoccluded_dets_mask
      result.init_azimuth_margin = the_interpolator.interpolate(low.init_azimuth_margin, high.init_azimuth_margin);
      result.range_margin        = the_interpolator.interpolate(low.range_margin, high.range_margin);
   }

   void CalibrationManager::interpolate_calibrations(const Interpolator &the_interpolator,
                                                     const Contour_Postprocessing_Calibrations_T &low,
                                                     const Contour_Postprocessing_Calibrations_T &high)
   {
      /* Always update when adding a new field to the Contour_Postprocessing_Calibrations_T structure. */
      static_assert((sizeof(Contour_Postprocessing_Calibrations_T) == 108U), "struct Contour_Postprocessing_Calibrations_T has "
                                                                             "changed, update interpolate_calibrations to "
                                                                             "interpolate and check new signal(s).");

      auto &result = m_resulting_calibration.contour_postprocessing;

      // get_occluded_contours
      result.declutter_contours.contour_occlusion_threshold = the_interpolator.interpolate(
         low.declutter_contours.contour_occlusion_threshold, high.declutter_contours.contour_occlusion_threshold);

      // get_occluded_vertices
      result.declutter_contours.contour_occlusion_azimuth_margin = the_interpolator.interpolate(
         low.declutter_contours.contour_occlusion_azimuth_margin, high.declutter_contours.contour_occlusion_azimuth_margin);
      result.declutter_contours.contour_occlusion_range_margin[0] = the_interpolator.interpolate(
         low.declutter_contours.contour_occlusion_range_margin[0], high.declutter_contours.contour_occlusion_range_margin[0]);
      result.declutter_contours.contour_occlusion_range_margin[1] = the_interpolator.interpolate(
         low.declutter_contours.contour_occlusion_range_margin[1], high.declutter_contours.contour_occlusion_range_margin[1]);

      // merge_contours
      result.merge_contours.merge_distance_threshold =
         the_interpolator.interpolate(low.merge_contours.merge_distance_threshold, high.merge_contours.merge_distance_threshold);
      result.merge_contours.merge_max_angle =
         the_interpolator.interpolate(low.merge_contours.merge_max_angle, high.merge_contours.merge_max_angle);
      result.merge_contours.merge_longitudinal_squeeze_factor = the_interpolator.interpolate(
         low.merge_contours.merge_longitudinal_squeeze_factor, high.merge_contours.merge_longitudinal_squeeze_factor);

      // presimplify_contours
      result.simplify_contours.presimplify_host_length =
         the_interpolator.interpolate(low.simplify_contours.presimplify_host_length, high.simplify_contours.presimplify_host_length);
      result.simplify_contours.presimplify_type =
         the_interpolator.interpolate(low.simplify_contours.presimplify_type, high.simplify_contours.presimplify_type);
      result.simplify_contours.presimplify_min_angle =
         the_interpolator.interpolate(low.simplify_contours.presimplify_min_angle, high.simplify_contours.presimplify_min_angle);
      result.simplify_contours.presimplify_max_angle =
         the_interpolator.interpolate(low.simplify_contours.presimplify_max_angle, high.simplify_contours.presimplify_max_angle);

      // simplify_contours
      result.simplify_contours.simplify_min_turning_angle = the_interpolator.interpolate(
         low.simplify_contours.simplify_min_turning_angle, high.simplify_contours.simplify_min_turning_angle);
      result.simplify_contours.simplify_type =
         the_interpolator.interpolate(low.simplify_contours.simplify_type, high.simplify_contours.simplify_type);
      result.simplify_contours.simplify_length_range[0] = the_interpolator.interpolate(
         low.simplify_contours.simplify_length_range[0], high.simplify_contours.simplify_length_range[0]);
      result.simplify_contours.simplify_length_range[1] = the_interpolator.interpolate(
         low.simplify_contours.simplify_length_range[1], high.simplify_contours.simplify_length_range[1]);
      result.simplify_contours.simplify_angle =
         the_interpolator.interpolate(low.simplify_contours.simplify_angle, high.simplify_contours.simplify_angle);

      // remove_vertices
      result.remove_vertices.remove_vertices_front_limit = the_interpolator.interpolate(
         low.remove_vertices.remove_vertices_front_limit, high.remove_vertices.remove_vertices_front_limit);
      result.remove_vertices.remove_vertices_rear_limit = the_interpolator.interpolate(
         low.remove_vertices.remove_vertices_rear_limit, high.remove_vertices.remove_vertices_rear_limit);
      result.remove_vertices.remove_vertices_left_limit = the_interpolator.interpolate(
         low.remove_vertices.remove_vertices_left_limit, high.remove_vertices.remove_vertices_left_limit);
      result.remove_vertices.remove_vertices_right_limit = the_interpolator.interpolate(
         low.remove_vertices.remove_vertices_right_limit, high.remove_vertices.remove_vertices_right_limit);
      result.remove_vertices.accepted_uncertainty =
         the_interpolator.interpolate(low.remove_vertices.accepted_uncertainty, high.remove_vertices.accepted_uncertainty);
      result.remove_vertices.max_num_cycles_no_update =
         the_interpolator.interpolate(low.remove_vertices.max_num_cycles_no_update, high.remove_vertices.max_num_cycles_no_update);

      // select and remove excess
      result.select_and_remove_excess_contours.max_num_contoured_clusters =
         the_interpolator.interpolate(low.select_and_remove_excess_contours.max_num_contoured_clusters,
                                      high.select_and_remove_excess_contours.max_num_contoured_clusters);
   }

   void CalibrationManager::interpolate_calibrations(const Interpolator &the_interpolator,
                                                     const SG_DC_Fusion_Calibrations_T &low,
                                                     const SG_DC_Fusion_Calibrations_T &high)
   {
      /* Always update when adding a new field to the SG_DC_Fusion_Calibrations_T structure. */
      static_assert((sizeof(SG_DC_Fusion_Calibrations_T) == 6U), "struct SG_DC_Fusion_Calibrations_T has "
                                                                 "changed, update interpolate_calibrations to "
                                                                 "interpolate and check new signal(s).");

      auto &result = m_resulting_calibration.sg_dc_fusion;
      // merge sg and dc contours
      result.max_num_vertices_per_fused_contour =
         the_interpolator.interpolate(low.max_num_vertices_per_fused_contour, high.max_num_vertices_per_fused_contour);
      result.max_num_fused_contours = the_interpolator.interpolate(low.max_num_fused_contours, high.max_num_fused_contours);
      result.max_num_fused_vertices = the_interpolator.interpolate(low.max_num_fused_vertices, high.max_num_fused_vertices);
   }

   void CalibrationManager::interpolate_calibrations(const Interpolator &the_interpolator,
                                                     const Time_Update_Calibrations_T &low,
                                                     const Time_Update_Calibrations_T &high)
   {
      /* Always update when adding a new field to the Time_Update_Calibrations_T structure. */
      static_assert((sizeof(Time_Update_Calibrations_T) == 16U), "struct Time_Update_Calibrations_T has "
                                                                 "changed, update interpolate_calibrations to "
                                                                 "interpolate and check new signal(s).");

      auto &result = m_resulting_calibration.time_update;
      result.noise.host_speed_weight_lat =
         the_interpolator.interpolate(low.noise.host_speed_weight_lat, high.noise.host_speed_weight_lat);
      result.noise.host_speed_weight_lon =
         the_interpolator.interpolate(low.noise.host_speed_weight_lon, high.noise.host_speed_weight_lon);
      result.noise.process_noise_base_std =
         the_interpolator.interpolate(low.noise.process_noise_base_std, high.noise.process_noise_base_std);
      result.noise.host_speed_weight = the_interpolator.interpolate(low.noise.host_speed_weight, high.noise.host_speed_weight);
   }

   void CalibrationManager::interpolate_calibrations(const Interpolator &the_interpolator,
                                                     const Drivability_Classification_Calibrations_T &low,
                                                     const Drivability_Classification_Calibrations_T &high)
   {
      /* Always update when adding a new field to the Drivability_Classification_Calibrations_T structure. */
      static_assert((sizeof(Drivability_Classification_Calibrations_T) == 116U), "struct "
                                                                                 "Drivability_Classification_Calibrations_T "
                                                                                 "has changed, update interpolate_calibrations to "
                                                                                 "interpolate and check new signal(s).");

      auto &result = m_resulting_calibration.drivability_classification;

      result.det_seg_assignment_threshold =
         the_interpolator.interpolate(low.det_seg_assignment_threshold, high.det_seg_assignment_threshold);

      // critical region for drivability classification
      for (uint8_t i = 0U; i < DC_NUM_POLYGON_VERTICES_BEFORE_INTERPOLATION; ++i)
      {
         result.critical_region_polygon[i].x =
            the_interpolator.interpolate(low.critical_region_polygon[i].x, high.critical_region_polygon[i].x);
         result.critical_region_polygon[i].y =
            the_interpolator.interpolate(low.critical_region_polygon[i].y, high.critical_region_polygon[i].y);
      }
      result.critical_region_subdivisions =
         the_interpolator.interpolate(low.critical_region_subdivisions, high.critical_region_subdivisions);
      result.critical_region_min_abs_curvature_threshold = the_interpolator.interpolate(
         low.critical_region_min_abs_curvature_threshold, high.critical_region_min_abs_curvature_threshold);
      result.critical_region_max_abs_curvature_threshold = the_interpolator.interpolate(
         low.critical_region_max_abs_curvature_threshold, high.critical_region_max_abs_curvature_threshold);
      result.subsegment_length = the_interpolator.interpolate(low.subsegment_length, high.subsegment_length);

      // update subsegments
      result.projection_distance_threshold =
         the_interpolator.interpolate(low.projection_distance_threshold, high.projection_distance_threshold);
      result.angle_between_segments_threshold =
         the_interpolator.interpolate(low.angle_between_segments_threshold, high.angle_between_segments_threshold);

      // subsegments classification
      result.min_num_dets_for_classification =
         the_interpolator.interpolate(low.min_num_dets_for_classification, high.min_num_dets_for_classification);
      result.overdrivable_max_z      = the_interpolator.interpolate(low.overdrivable_max_z, high.overdrivable_max_z);
      result.underdrivable_min_z     = the_interpolator.interpolate(low.underdrivable_min_z, high.underdrivable_min_z);
      result.underdrivable_min_z_std = the_interpolator.interpolate(low.underdrivable_min_z_std, high.underdrivable_min_z_std);
      result.decision_tree_obstacle_prob_thr =
         the_interpolator.interpolate(low.decision_tree_obstacle_prob_thr, high.decision_tree_obstacle_prob_thr);
      result.decision_tree_overdrivability_obstacle_prob_thr = the_interpolator.interpolate(
         low.decision_tree_overdrivability_obstacle_prob_thr, high.decision_tree_overdrivability_obstacle_prob_thr);

      // features calculation
      result.features_inliers_threshold =
         the_interpolator.interpolate(low.features_inliers_threshold, high.features_inliers_threshold);
      result.features_radar_height    = the_interpolator.interpolate(low.features_radar_height, high.features_radar_height);
      result.features_z_max_threshold = the_interpolator.interpolate(low.features_z_max_threshold, high.features_z_max_threshold);
      result.features_z_min_threshold = the_interpolator.interpolate(low.features_z_min_threshold, high.features_z_min_threshold);
      result.features_max_azimuth_confidence =
         the_interpolator.interpolate(low.features_max_azimuth_confidence, high.features_max_azimuth_confidence);
      result.features_max_elevation_confidence =
         the_interpolator.interpolate(low.features_max_elevation_confidence, high.features_max_elevation_confidence);
   }

   void CalibrationManager::interpolate_calibrations(const Interpolator &the_interpolator,
                                                     const Contour_Downselection_Calibrations_T &low,
                                                     const Contour_Downselection_Calibrations_T &high)
   {
      /* Always update when adding a new field to the Contour_Downselection_Calibrations_T structure. */
      static_assert((sizeof(Contour_Downselection_Calibrations_T) == 64U), "struct Contour_Downselection_Calibrations_T has "
                                                                           "changed, update interpolate_calibrations to "
                                                                           "interpolate and check new signal(s).");
      auto &result = m_resulting_calibration.contour_downselection;
      result.curvi_roi_curvature_threshold =
         the_interpolator.interpolate(low.curvi_roi_curvature_threshold, high.curvi_roi_curvature_threshold);
      result.roi_front_length_vcs     = the_interpolator.interpolate(low.roi_front_length_vcs, high.roi_front_length_vcs);
      result.roi_half_width_vcs       = the_interpolator.interpolate(low.roi_half_width_vcs, high.roi_half_width_vcs);
      result.roi_rear_length_vcs      = the_interpolator.interpolate(low.roi_rear_length_vcs, high.roi_rear_length_vcs);
      result.contour_priority_shift_x = the_interpolator.interpolate(low.contour_priority_shift_x, high.contour_priority_shift_x);
      result.contour_priority_compaction_factor_x =
         the_interpolator.interpolate(low.contour_priority_compaction_factor_x, high.contour_priority_compaction_factor_x);
      result.contour_priority_compaction_factor_y =
         the_interpolator.interpolate(low.contour_priority_compaction_factor_y, high.contour_priority_compaction_factor_y);
      result.min_number_of_valid_vertices =
         the_interpolator.interpolate(low.min_number_of_valid_vertices, high.min_number_of_valid_vertices);
      result.min_x_to_downselect_contour =
         the_interpolator.interpolate(low.min_x_to_downselect_contour, high.min_x_to_downselect_contour);
      result.min_required_underdrivable_share_to_reject_contour = the_interpolator.interpolate(
         low.min_required_underdrivable_share_to_reject_contour, high.min_required_underdrivable_share_to_reject_contour);
      result.min_required_overdrivable_share_to_reject_contour = the_interpolator.interpolate(
         low.min_required_overdrivable_share_to_reject_contour, high.min_required_overdrivable_share_to_reject_contour);
      result.min_underdrivable_share_along_with_unclassified = the_interpolator.interpolate(
         low.min_underdrivable_share_along_with_unclassified, high.min_underdrivable_share_along_with_unclassified);
      result.min_unclassified_share_along_with_underdrivable = the_interpolator.interpolate(
         low.min_unclassified_share_along_with_underdrivable, high.min_unclassified_share_along_with_underdrivable);
      result.min_required_contour_priority_to_downselect = the_interpolator.interpolate(
         low.min_required_contour_priority_to_downselect, high.min_required_contour_priority_to_downselect);
      result.importance_decay_coef = the_interpolator.interpolate(low.importance_decay_coef, high.importance_decay_coef);
      result.position_importance_saturation =
         the_interpolator.interpolate(low.position_importance_saturation, high.position_importance_saturation);
   }


   void CalibrationManager::assign_current_calibration(const CalibrationIndices index_low,
                                                       const CalibrationIndices index_high,
                                                       const float speed_current)
   {
      if (index_low == index_high)
      {
         m_resulting_calibration = m_calibration_lut[static_cast<std::uint8_t>(index_low)];
      }
      else // Do an interpolation
      {
         const Calibrations_T &low  = m_calibration_lut[static_cast<std::uint8_t>(index_low)];
         const Calibrations_T &high = m_calibration_lut[static_cast<std::uint8_t>(index_high)];
         const float speed_low      = m_velocity_thresholds[static_cast<std::uint8_t>(index_low)];
         const float speed_high     = m_velocity_thresholds[static_cast<std::uint8_t>(index_high)];
         const Interpolator the_interpolator(speed_low, speed_high, speed_current);

         interpolate_calibrations(the_interpolator, low.time_update, high.time_update);
         interpolate_calibrations(the_interpolator, low.common, high.common);
         interpolate_calibrations(the_interpolator, low.detection_processing, high.detection_processing);
         interpolate_calibrations(the_interpolator, low.detection_clustering, high.detection_clustering);
         interpolate_calibrations(the_interpolator, low.measurement_association, high.measurement_association);
         interpolate_calibrations(the_interpolator, low.measurement_update, high.measurement_update);
         interpolate_calibrations(the_interpolator, low.contour_initialization, high.contour_initialization);
         interpolate_calibrations(the_interpolator, low.contour_postprocessing, high.contour_postprocessing);
         interpolate_calibrations(the_interpolator, low.drivability_classification, high.drivability_classification);
         interpolate_calibrations(the_interpolator, low.sg_dc_fusion, high.sg_dc_fusion);
         interpolate_calibrations(the_interpolator, low.contour_downselection, high.contour_downselection);
      }
   }
}
