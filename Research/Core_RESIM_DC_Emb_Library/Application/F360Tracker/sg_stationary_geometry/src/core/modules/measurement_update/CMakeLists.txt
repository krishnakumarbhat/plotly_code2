cmake_minimum_required(VERSION 3.16)

add_library(SG-Measurement-Update OBJECT)

target_include_directories(
   SG-Measurement-Update
      PUBLIC iface
      PRIVATE inc
)

target_sources(
   SG-Measurement-Update
      PRIVATE
         inc/sg_measurement_update_utils.h
         inc/sg_position_measurement_update.h
         inc/sg_length_measurement_update.h
         iface/sg_measurement_update_contours.h
         src/sg_measurement_update_utils.cpp
         src/sg_position_measurement_update.cpp
         src/sg_length_measurement_update.cpp
         src/sg_measurement_update_contours.cpp
)

target_link_libraries(
   SG-Measurement-Update
      PRIVATE sg_compiler_config
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Common
      PRIVATE SG-Host
      PRIVATE SG-Calibrations
)

get_target_property(src_files SG-Measurement-Update SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})