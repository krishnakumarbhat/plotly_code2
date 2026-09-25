cmake_minimum_required(VERSION 3.10)

project(Radar_Sensor_Preprocessing)

set(SRC
   source/rspp_check_det_az_conf_and_elevation.cpp
   source/rspp_check_det_az_conf_and_elevation.h
   source/rspp_compute_detections_properties.cpp
   source/rspp_compute_detections_properties.h
   source/rspp_detection_multipath_filter.cpp
   source/rspp_detection_multipath_filter.h
   source/rspp_ego_motion_compensate_dets.cpp
   source/rspp_ego_motion_compensate_dets.h
   source/rspp_filter_detections_with_bad_azimuth_confidence.cpp
   source/rspp_filter_detections_with_bad_azimuth_confidence.h
   source/rspp_functional.h
   source/rspp_get_version.cpp
   source/rspp_input_sanity_check.cpp
   source/rspp_input_sanity_check.h
   source/rspp_inputs_preprocessing.cpp
   source/rspp_iterator.h
   source/rspp_iterator_detail.h
   source/rspp_math_func.cpp
   source/rspp_math_func.h
   source/rspp_norm_heading_angle.cpp
   source/rspp_norm_heading_angle.h
   source/rspp_sensor_capability.cpp
   source/rspp_sensor_capability.h
   source/rspp_set_default_flags_raw_detections.cpp
   source/rspp_set_default_flags_raw_detections.h
   source/rspp_sort_data_type.h
   source/rspp_statistical_tests.cpp
   source/rspp_statistical_tests.h
   source/rspp_uncertainty_propagation.cpp
   source/rspp_uncertainty_propagation.h
   source/rspp_update_sensor_first_detection_index.cpp
   source/rspp_update_sensor_first_detection_index.h
   source/rspp_update_sensors.cpp
   source/rspp_update_sensors.h
   source/rspp_update_sensors_fov.cpp
   source/rspp_update_sensors_fov.h
   source/rspp_update_sensors_time.cpp
   source/rspp_update_sensors_time.h
   source/rspp_update_sensors_valid_info.cpp
   source/rspp_update_sensors_valid_info.h
   source/rspp_vcs_long_sorted_dets_support_functions.cpp
   source/rspp_vcs_long_sorted_dets_support_functions.h
   source/rspp_calibrations.cpp
   source/rspp_host_props.h
   source/rspp_detection_double_bounce_detector.cpp
   source/rspp_detection_double_bounce_detector.h
   source/rspp_try_to_dealiase_range_rate.cpp
   source/rspp_try_to_dealiase_range_rate.h
   source/rspp_discard_detections_outside_sensor_fov.cpp
   source/rspp_discard_detections_outside_sensor_fov.h
)

set(IFACE_SRC
   include/rspp_calibrations.h
   include/rspp_constants.h
   include/rspp_core_info.h
   include/rspp_detection.h
   include/rspp_detection_list.h
   include/rspp_detection_motion_status.h
   include/rspp_get_version.h
   include/rspp_host.h
   include/rspp_inputs_preprocessing.h
   include/rspp_look_ID.h
   include/rspp_look_type.h
   include/rspp_math.h
   include/rspp_mounting_location.h
   include/rspp_position.h
   include/rspp_radar_sensor.h
   include/rspp_range_type.h
   include/rspp_reuse.h
   include/rspp_sensor_mounting_position.h
   include/rspp_sensor_type.h
   include/rspp_variant_definition.h
   include/rspp_variant_type.h
   include/rspp_velocity.h
   include/rspp_version.h
)


add_library(rspp ${SRC})

target_include_directories(rspp
   PUBLIC
      include
   PRIVATE 
      source
)

add_subdirectory(include)
include(variant_config.cmake)

# Set up SW tag
set(RSPP_SW_TAG_HEX "0x0000000" CACHE STRING "GIT commit hash")
message("RSPP SW tag: " ${RSPP_SW_TAG_HEX})
add_definitions(-DRSPP_VERSION_BUILD_ID=${RSPP_SW_TAG_HEX})

# Set up compile options
if (MSVC)
   add_definitions(-D_SCL_SECURE_NO_WARNINGS)
   target_compile_options( rspp PRIVATE
      /W4 # warning level 4
      /WX # treat warnings as errors
   )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   target_compile_options( rspp PRIVATE
      -Wall
      -Wextra
      -pedantic
      -Wnon-virtual-dtor
      -Wswitch-default
      -Werror
   )
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC} ${IFACE_SRC})

# Unit testing, coverage and docs
option(RSPP_UT "RSPP UT framework" OFF)
if(RSPP_UT)
    include(ut_config.cmake)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   option(RSPP_coverage "RSPP coverage" OFF)
   option(RSPP_docs "RSPP test documentation" OFF)
   if(RSPP_coverage)
      include(coverage_config.cmake)
   endif()
   if(RSPP_docs)
      include(rspp_docs.cmake)
   endif()
endif()
