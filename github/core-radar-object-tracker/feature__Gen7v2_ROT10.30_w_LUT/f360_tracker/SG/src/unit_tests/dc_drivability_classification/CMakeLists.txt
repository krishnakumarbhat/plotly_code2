cmake_minimum_required(VERSION 3.16)

add_executable(dc_drivability_classification_ut)

target_sources(
   dc_drivability_classification_ut
      PRIVATE
         ut_dc_amend_segment.cpp
         ut_dc_append_remaining_sg_contours.cpp
         ut_dc_assign_closest_subsegments.cpp
         ut_dc_assign_detections_to_subsegments_and_update_features.cpp
         ut_dc_calculate_subsegment_features.cpp
         ut_dc_consolidate_subsegment_drivability.cpp
         ut_dc_contour.cpp
         ut_dc_contour_fusion.cpp
         ut_dc_contour_storage.cpp
         ut_dc_create_critical_region.cpp
         ut_dc_curvilinear_transformation.cpp
         ut_dc_decision_tree.cpp
         ut_dc_decision_tree_overdrivability.cpp
         ut_dc_extend_critical_region.cpp
         ut_dc_features_calculator.cpp
         ut_dc_get_subvertices_positions.cpp
         ut_dc_interpolate_polygon.cpp
         ut_dc_interpolate_segment.cpp
         ut_dc_is_vertex_in_critical_region.cpp
         ut_dc_map_sg_covariance_to_dc_vertices.cpp
         ut_dc_past_data.cpp
         ut_dc_project_points_on_line.cpp
         ut_dc_time_update_subsegments.cpp
         ut_dc_update_subsegments.cpp
         ut_dc_update_subsegments_drivability.cpp
         ut_dc_update_subsegments_helpers.cpp
)

target_link_libraries(
   dc_drivability_classification_ut
      PRIVATE DC-Drivability-Classification
      PRIVATE DC-Dummy-Generator
      PRIVATE SG-Calibrations
      PRIVATE SG-Common
      PRIVATE SG-Math
      PRIVATE sg_ut_config
      PRIVATE stationary_geometries_iface
)

target_include_directories(
   dc_drivability_classification_ut
      PRIVATE $<TARGET_PROPERTY:DC-Drivability-Classification,INCLUDE_DIRECTORIES>
)

include(GoogleTest)
gtest_discover_tests(dc_drivability_classification_ut)
