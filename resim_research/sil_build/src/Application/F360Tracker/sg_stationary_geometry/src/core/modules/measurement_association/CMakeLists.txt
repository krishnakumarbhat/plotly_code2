cmake_minimum_required(VERSION 3.16)

add_library(SG-Measurement-Association OBJECT)

target_include_directories(
   SG-Measurement-Association
      PUBLIC iface
      PRIVATE inc
)

target_sources(
   SG-Measurement-Association
      PRIVATE
         inc/sg_measurement_association_utils.h
         inc/sg_associate_detections_to_contour.h
         iface/sg_measurement_association.h
         src/sg_measurement_association.cpp
         src/sg_measurement_association_utils.cpp
         src/sg_associate_detections_to_contour.cpp
)

target_link_libraries(
   SG-Measurement-Association
      PRIVATE sg_compiler_config
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Common
      PRIVATE SG-Math
      PRIVATE SG-Calibrations
)

get_target_property(src_files SG-Measurement-Association SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})