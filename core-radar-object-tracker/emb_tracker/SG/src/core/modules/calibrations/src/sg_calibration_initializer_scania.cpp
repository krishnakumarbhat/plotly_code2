#include "sg_calibration_initializer_scania.h"

namespace sg
{
void CalibrationInitializerScania::initialize(CommonCalibs &calibrations, const Scenario &chosen_scenario) const
{
   CalibrationInitializerPlatform::initialize(calibrations, chosen_scenario);
   calibrations.view_ranges.nondrivable.lateral.min              = -13.0F;
   calibrations.view_ranges.nondrivable.lateral.max              = 13.0F;
   calibrations.view_ranges.nondrivable.longitudinal.min         = set_calibration(chosen_scenario, -30.0F, -5.0F, 4.0F);
   calibrations.view_ranges.nondrivable.longitudinal.max         = set_calibration(chosen_scenario, 30.0F, 60.0F, 60.0F);
   calibrations.view_ranges.nondrivable.vertical.overground.min  = 0.0F;
   calibrations.view_ranges.nondrivable.vertical.overground.max  = 2.0F;
   calibrations.view_ranges.nondrivable.vertical.underground.min = -2.0F;
   calibrations.view_ranges.nondrivable.vertical.underground.max = -0.0F;
   calibrations.view_ranges.overdrivable.longitudinal.max        = 80.0F;
   calibrations.view_ranges.underdrivable.longitudinal.max       = 80.0F;
}

void CalibrationInitializerScania::initialize(DetectionProcessingCalibs &calibrations, const Scenario &chosen_scenario) const
{
   CalibrationInitializerPlatform::initialize(calibrations, chosen_scenario);
   calibrations.importance_calibrations.age_impact_factor        = 0.0F;
   calibrations.importance_calibrations.host_speed_impact_factor = 0.2F;
}

void CalibrationInitializerScania::initialize(DetectionClusteringCalibs &calibrations, const Scenario &chosen_scenario) const
{
   CalibrationInitializerPlatform::initialize(calibrations, chosen_scenario);
   calibrations.linear_piecewise_transform_coefficients.longitudinal.nondrivable[0U].range = 30.0F;
   calibrations.linear_piecewise_transform_coefficients.longitudinal.nondrivable[0U].factor =
      set_calibration(chosen_scenario, 0.5F, 0.5F, 0.1F);
   calibrations.linear_piecewise_transform_coefficients.longitudinal.nondrivable[1U].range = 60.0F;
   calibrations.linear_piecewise_transform_coefficients.longitudinal.nondrivable[1U].factor =
      set_calibration(chosen_scenario, 0.5F, 0.5F, 0.1F);
   calibrations.linear_piecewise_transform_coefficients.longitudinal.nondrivable[2U].range = 100.0F;
   calibrations.linear_piecewise_transform_coefficients.longitudinal.nondrivable[2U].factor =
      set_calibration(chosen_scenario, 0.4F, 0.4F, 0.1F);
   calibrations.cluster_detections.cluster_radius_underdrivable = 2.9F;
}

void CalibrationInitializerScania::initialize(MeasurementAssociationCalibs &calibrations, const Scenario &chosen_scenario) const
{
   CalibrationInitializerPlatform::initialize(calibrations, chosen_scenario);
   calibrations.width_min           = 2.6F;
   calibrations.width_max           = 5.9F;
   calibrations.length_margin_min   = 0.0F;
   calibrations.f_dynamic_gates     = false;
   calibrations.width_extension_min = 1.5F;
   calibrations.width_extension_max = 6.0F;
}

void CalibrationInitializerScania::initialize(MeasurementUpdateCalibs &calibrations, const Scenario &chosen_scenario) const
{
   CalibrationInitializerPlatform::initialize(calibrations, chosen_scenario);
   calibrations.regularity_weight = set_calibration(chosen_scenario, 1.0F, 1.0F, 5.0F);
}

void CalibrationInitializerScania::initialize(ContourInitializationCalibs &calibrations, const Scenario &chosen_scenario) const
{
   CalibrationInitializerPlatform::initialize(calibrations, chosen_scenario);
   calibrations.cluster_priority_shift_x                = set_calibration(chosen_scenario, 0.0F, 15.0F, 40.0F);
   calibrations.cluster_priority_compaction_factor_y    = set_calibration(chosen_scenario, 3.0F, 1.5F, 0.8F);
   calibrations.min_dets_span_for_stationary_hypothesis = 15.0F;
   calibrations.min_num_dets_for_stationary_hypothesis  = 13U;
   calibrations.max_det_age_for_contour_init            = set_calibration(chosen_scenario, 3U, 6U, 6U);
}

void CalibrationInitializerScania::initialize(ContourPostprocessingCalibs &calibrations, const Scenario &chosen_scenario) const
{
   CalibrationInitializerPlatform::initialize(calibrations, chosen_scenario);
   calibrations.remove_vertices.remove_vertices_front_limit = 60.0F;
   calibrations.remove_vertices.remove_vertices_rear_limit  = set_calibration(chosen_scenario, -30.0F, -20.0F, -20.0F);
   calibrations.remove_vertices.remove_vertices_left_limit  = -13.0F;
   calibrations.remove_vertices.remove_vertices_right_limit = 13.0F;
}

void CalibrationInitializerScania::initialize(ContourDownselectionCalibs &calibrations, const Scenario &chosen_scenario) const
{
   CalibrationInitializerPlatform::initialize(calibrations, chosen_scenario);
   calibrations.contour_priority_shift_x             = set_calibration(chosen_scenario, 0.0F, 15.0F, 40.0F);
   calibrations.contour_priority_compaction_factor_y = set_calibration(chosen_scenario, 3.0F, 1.5F, 0.8F);
   calibrations.min_x_to_downselect_contour          = -5.0F;
   calibrations.position_importance_saturation       = 3.0F; // This is a highest squared x_span value, above which all
   calibrations.contour_length_sq_threshold          = set_calibration(chosen_scenario, 1.0F, 0.0F, 0.0F);
}
} // namespace sg
