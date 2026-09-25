cmake_minimum_required(VERSION 3.16)

add_library(SG-Rspp OBJECT)
add_subdirectory(iface)

target_include_directories(
   SG-Rspp
      PUBLIC iface
      PUBLIC inc
      PRIVATE src
)

target_sources(
   SG-Rspp
      PUBLIC
         iface/rspp_calibrations.h
         iface/rspp_constants.h
         iface/rspp_core_info.h
         iface/rspp_detection.h
         iface/rspp_detection_list.h
         iface/rspp_detection_motion_status.h
         iface/rspp_get_version.h
         iface/rspp.h
         iface/rspp_host.h
         iface/rspp_look_ID.h
         iface/rspp_look_type.h
         iface/rspp_math.h
         iface/rspp_mounting_location.h
         iface/rspp_position.h
         iface/rspp_radar_sensor.h
         iface/rspp_range_type.h
         iface/rspp_reuse.h
         iface/rspp_sensor_mounting_position.h
         iface/rspp_sensor_type.h
         iface/rspp_types.h
         iface/rspp_variant_definition.h
         iface/rspp_variant_type.h
         iface/rspp_velocity.h
         iface/rspp_version.h
      PRIVATE
         inc/rspp_coordinate_transformation.h
         inc/rspp_functional.h
         inc/rspp_input_validation.h
         inc/rspp_internal.h
         inc/rspp_internal_types.h
         inc/rspp_iterator_detail.h
         inc/rspp_iterator.h
         inc/rspp_mark_detection_with_low_detection_conf.h
         inc/rspp_motion_status_classification.h
         inc/rspp_norm_heading_angle.h
         inc/rspp_range_rate_compensation.h
         inc/rspp_math_func.h
         inc/rspp_sensor_capability.h
         inc/rspp_sort_data_type.h
         inc/rspp_state.h
         inc/rspp_uncertainty_calculation.h
         inc/rspp_vcs_long_sorted_dets_support_functions.h
      PRIVATE
         src/rspp_calibrations.cpp
         src/rspp_coordinate_transformation.cpp
         src/rspp.cpp
         src/rspp_get_version.cpp
         src/rspp_input_validation.cpp
         src/rspp_mark_detection_with_low_detection_conf.cpp
         src/rspp_math_func.cpp
         src/rspp_motion_status_classification.cpp
         src/rspp_norm_heading_angle.cpp
         src/rspp_range_rate_compensation.cpp
         src/rspp_sensor_capability.cpp
         src/rspp_state.cpp
         src/rspp_uncertainty_calculation.cpp
         src/rspp_vcs_long_sorted_dets_support_functions.cpp
)

target_link_libraries(
   SG-Rspp
      PRIVATE sg_compiler_config
)

get_target_property(src_files SG-Rspp SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
