cmake_minimum_required(VERSION 3.16)

add_library(SG-Contour-Downselection OBJECT)

target_include_directories(
   SG-Contour-Downselection
      PUBLIC iface
      PRIVATE inc
)

target_sources(
   SG-Contour-Downselection
      PRIVATE
         iface/sg_contour_downselection.h
         src/sg_contour_downselection.cpp
)

target_link_libraries(
   SG-Contour-Downselection
      PRIVATE sg_compiler_config
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Common
      PRIVATE SG-Calibrations
      PRIVATE DC-Drivability-Classification
)

get_target_property(src_files SG-Contour-Downselection SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})