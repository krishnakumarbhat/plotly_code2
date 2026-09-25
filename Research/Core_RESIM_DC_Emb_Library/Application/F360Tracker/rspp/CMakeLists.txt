cmake_minimum_required(VERSION 3.10)

project(Radar_Sensor_Preprocessing)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../utilities/cmake/ConfigureAddressSanitizer.cmake)
configure_address_sanitizer()

set(SRC
   source/rspp_coordinate_transformation.cpp
   source/rspp_coordinate_transformation.h
   source/rspp_functional.h
   source/rspp_get_version.cpp
   source/rspp_input_validation.cpp
   source/rspp_input_validation.h
   source/rspp_internal.h
   source/rspp_internal_types.h
   source/rspp_iterator_detail.h
   source/rspp_iterator.h
   source/rspp_mark_detection_with_low_detection_conf.cpp
   source/rspp_mark_detection_with_low_detection_conf.h
   source/rspp_math_func.cpp
   source/rspp_math_func.h
   source/rspp_motion_status_classification.cpp
   source/rspp_motion_status_classification.h
   source/rspp_norm_heading_angle.cpp
   source/rspp_norm_heading_angle.h
   source/rspp_range_rate_compensation.cpp
   source/rspp_range_rate_compensation.h
   source/rspp_sensor_capability.cpp
   source/rspp_sensor_capability.h
   source/rspp_sort_data_type.h
   source/rspp_state.cpp
   source/rspp_state.h
   source/rspp_uncertainty_calculation.cpp
   source/rspp_uncertainty_calculation.h
   source/rspp_vcs_long_sorted_dets_support_functions.cpp
   source/rspp_vcs_long_sorted_dets_support_functions.h
   source/rspp.cpp
)

set(IFACE_SRC
   include/rspp_constants.h
   include/rspp_core_info.h
   include/rspp_detection.h
   include/rspp_detection_list.h
   include/rspp_detection_motion_status.h
   include/rspp_get_version.h
   include/rspp_host.h
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
   include/rspp_types.h
   include/rspp_variant_definition.h
   include/rspp_variant_type.h
   include/rspp_velocity.h
   include/rspp_version.h
   include/rspp.h
)

set(RSPP_ALL_SOURCES ${SRC} ${IFACE_SRC})

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "C6000" AND CMAKE_BUILD_TYPE STREQUAL "Release")
   add_c6x_library(
      TARGET rspp
      SOURCES ${RSPP_ALL_SOURCES}
   )
else()
   add_library(rspp ${RSPP_ALL_SOURCES})
endif()

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

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${RSPP_ALL_SOURCES})

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
