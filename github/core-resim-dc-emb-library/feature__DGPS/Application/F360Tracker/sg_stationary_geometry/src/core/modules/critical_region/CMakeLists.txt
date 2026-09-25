cmake_minimum_required(VERSION 3.16)

add_library(SG-Critical_Region OBJECT)

target_include_directories(
   SG-Critical_Region
      PUBLIC iface
)

target_sources(
   SG-Critical_Region
      PRIVATE
         iface/sg_critical_region.h
         src/sg_critical_region.cpp
)

target_link_libraries(
   SG-Critical_Region
      PRIVATE sg_compiler_config
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Common
      PRIVATE SG-Calibrations
      PRIVATE SG-Math
)

get_target_property(src_files SG-Critical_Region SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})