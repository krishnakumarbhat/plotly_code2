cmake_minimum_required(VERSION 3.16)

add_library(occupancy_grid STATIC)

target_sources(occupancy_grid PRIVATE ocg_occupancy_grid.cpp)

if(${OCG_VARIANT_NAME} STREQUAL "FLR4px1_v1")
  target_sources(
    occupancy_grid
    PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/ocg_calibrations/FLR4px1_v1/ocg_calibrations.cpp
  )

elseif(${OCG_VARIANT_NAME} STREQUAL "FLR4x1_v1")
  target_sources(
    occupancy_grid
    PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/ocg_calibrations/FLR4x1_v1/ocg_calibrations.cpp)

elseif(${OCG_VARIANT_NAME} STREQUAL "FLR4p_plt_x1_v1")
  target_sources(
    occupancy_grid
    PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/ocg_calibrations/FLR4p_plt_x1_v1/ocg_calibrations.cpp)

elseif(${OCG_VARIANT_NAME} STREQUAL "FLR7x1_v1")
  target_sources(
    occupancy_grid
    PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/ocg_calibrations/FLR7x1_v1/ocg_calibrations.cpp)

elseif(${OCG_VARIANT_NAME} STREQUAL "SRR7px2_v1")
  target_sources(
    occupancy_grid
    PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/ocg_calibrations/SRR7px2_v1/ocg_calibrations.cpp)
endif()

add_subdirectory(common)
add_subdirectory(underdrivability)

target_link_libraries(occupancy_grid PRIVATE ocg_underdrivability ocg_common)

target_link_libraries(occupancy_grid PUBLIC occupancy_grid_iface)
