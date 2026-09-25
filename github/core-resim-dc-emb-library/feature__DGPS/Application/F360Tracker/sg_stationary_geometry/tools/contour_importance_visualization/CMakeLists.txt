cmake_minimum_required(VERSION 3.16)
project(SaveContourImportance)

add_executable(save_contour_importance_to_csv)

target_sources(
   save_contour_importance_to_csv
      PRIVATE
         save_contour_importance.cpp
         save_contour_importance_helpers.cpp
         save_contour_importance_helpers.h
)

add_subdirectory("../.." "SGCore")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../components/rot "rot")


target_link_libraries(
   save_contour_importance_to_csv
      PRIVATE SG-Common
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Contour-Downselection
      PRIVATE SG-Math
      PRIVATE SG-Host
      PRIVATE SG-Calibrations
      PRIVATE DC-Drivability-Classification
      PRIVATE rspp_iface
)