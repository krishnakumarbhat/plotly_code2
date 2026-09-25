cmake_minimum_required(VERSION 3.16)

add_library(DC-Drivability-Classification-Wrapper OBJECT)

target_include_directories(
   DC-Drivability-Classification-Wrapper
      PUBLIC iface
      PRIVATE inc
)

target_sources(
   DC-Drivability-Classification-Wrapper
      PRIVATE
         iface/dc_additional_features_calculator.h
         src/dc_additional_features_calculator.cpp
)

target_link_libraries(
   DC-Drivability-Classification-Wrapper
      PRIVATE sg_compiler_config
      PRIVATE DC-Drivability-Classification
      PRIVATE stationary_geometries
)

get_target_property(src_files DC-Drivability-Classification-Wrapper SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})