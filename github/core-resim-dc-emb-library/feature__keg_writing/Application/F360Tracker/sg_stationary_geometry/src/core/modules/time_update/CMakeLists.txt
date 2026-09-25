cmake_minimum_required(VERSION 3.16)

add_library(SG-Time-Update OBJECT)

target_include_directories(
   SG-Time-Update
      PUBLIC iface
      PRIVATE inc
)

target_sources(
   SG-Time-Update
      PRIVATE
         iface/sg_time_update.h
         iface/sg_time_update_detections.h
         iface/sg_time_update_contours.h
         inc/sg_time_update_utils.h
         inc/sg_host_displacement.h
         src/sg_time_update.cpp
         src/sg_time_update_detections.cpp
         src/sg_time_update_utils.cpp
         src/sg_time_update_contours.cpp
)

target_link_libraries(
   SG-Time-Update
      PRIVATE sg_compiler_config
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Common
      PRIVATE SG-Host
      PRIVATE SG-Calibrations
)

get_target_property(src_files SG-Time-Update SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})