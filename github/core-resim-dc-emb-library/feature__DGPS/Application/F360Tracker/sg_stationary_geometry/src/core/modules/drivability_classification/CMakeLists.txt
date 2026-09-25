cmake_minimum_required(VERSION 3.16)

add_library(DC-Drivability-Classification OBJECT)

target_include_directories(
   DC-Drivability-Classification
      PUBLIC iface
      PRIVATE inc
)

target_sources(
   DC-Drivability-Classification
      PRIVATE
         iface/dc_contour.h
         iface/dc_contour_fusion.h
         iface/dc_contour_storage.h
         iface/dc_features.h
         iface/dc_past_data.h
         iface/dc_subsegment.h
         iface/dc_subsegment_controller.h
         iface/dc_subsegment_vertex.h
         iface/dc_subsegment_detections.h
         inc/dc_amend_segment.h
         inc/dc_append_remaining_sg_contours.h
         inc/dc_array_wrapper.h
         inc/dc_assign_closest_subsegments.h
         inc/dc_assign_detections_to_subsegments_and_update_features.h
         inc/dc_calculate_subsegment_features.h
         inc/dc_common.h
         inc/dc_consolidate_subsegment_drivability.h
         inc/dc_decision_tree.h
         inc/dc_decision_tree_overdrivability.h
         inc/dc_features_calculator.h
         inc/dc_get_subvertices_positions.h
         inc/dc_split_segment_to_subsegments.h
         inc/dc_interpolate_segment.h
         inc/dc_map_sg_covariance_to_dc_vertices.h
         inc/dc_project_points_on_line.h
         inc/dc_time_update_subsegments.h
         inc/dc_update_subsegments.h
         inc/dc_update_subsegments_drivability.h
         inc/dc_update_subsegments_helpers.h
         src/dc_subsegment_controller.cpp
         src/dc_amend_segment.cpp
         src/dc_append_remaining_sg_contours.cpp
         src/dc_assign_closest_subsegments.cpp
         src/dc_assign_detections_to_subsegments_and_update_features.cpp
         src/dc_calculate_subsegment_features.cpp
         src/dc_consolidate_subsegment_drivability.cpp
         src/dc_contour.cpp
         src/dc_contour_fusion.cpp
         src/dc_contour_storage.cpp
         src/dc_decision_tree.cpp
         src/dc_decision_tree_overdrivability.cpp
         src/dc_features_calculator.cpp
         src/dc_get_subvertices_positions.cpp
         src/dc_split_segment_to_subsegments.cpp
         src/dc_interpolate_segment.cpp
         src/dc_map_sg_covariance_to_dc_vertices.cpp
         src/dc_past_data.cpp
         src/dc_project_points_on_line.cpp
         src/dc_subsegment.cpp
         src/dc_subsegment_vertex.cpp
         src/dc_time_update_subsegments.cpp
         src/dc_update_subsegments.cpp
         src/dc_update_subsegments_drivability.cpp
         src/dc_update_subsegments_helpers.cpp
)

target_link_libraries(
   DC-Drivability-Classification
      PRIVATE sg_compiler_config
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Critical_Region
      PRIVATE SG-Common
      PRIVATE SG-Calibrations
      PRIVATE SG-Host
      PRIVATE SG-Math
)

get_target_property(src_files DC-Drivability-Classification SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
