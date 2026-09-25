cmake_minimum_required(VERSION 3.16)

add_library(ocg_underdrivability STATIC)

target_include_directories(
  ocg_underdrivability
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/iface
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(
  ocg_underdrivability
  PRIVATE source/ocg_underdrivability.cpp
          source/ocg_process_detections.cpp
          source/ocg_initialize_underdrivability.cpp
          source/ocg_shift_circular_zones.cpp
          source/ocg_assign_detections_to_underdrivability_zones.cpp
          source/ocg_assign_detections_to_underdrivability_zones_helpers.cpp
          source/ocg_calc_underdrivability_probabilities.cpp
          source/ocg_assign_underdrivability_status_to_zones.cpp
          source/ocg_test_highly_critical_zones.cpp
          source/ocg_test_med_critical_zones.cpp
          source/ocg_test_low_critical_zones.cpp
          source/ocg_calc_underdrivability_probabilities_helpers.cpp)

target_link_libraries(ocg_underdrivability PRIVATE ocg_common)

target_link_libraries(ocg_underdrivability PUBLIC occupancy_grid_iface)

get_target_property(src_files ocg_underdrivability SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})
