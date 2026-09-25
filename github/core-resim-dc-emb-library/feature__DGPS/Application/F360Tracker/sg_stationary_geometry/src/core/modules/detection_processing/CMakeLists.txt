cmake_minimum_required(VERSION 3.16)

add_library(SG-Detection-Processing OBJECT)

target_include_directories(
   SG-Detection-Processing
      PUBLIC iface
      PRIVATE inc
)

target_sources(
   SG-Detection-Processing
      PRIVATE
         iface/sg_combine_detections.h
         iface/sg_determine_drivability_class.h
         iface/sg_downselect_input_detections.h
         inc/sg_combine_detections_helpers.h
         inc/sg_calculate_detection_importance.h
         inc/sg_downselect_input_detections_helpers.h
         src/sg_combine_detections.cpp
         src/sg_combine_detections_helpers.cpp
         src/sg_calculate_detection_importance.cpp
         src/sg_downselect_input_detections.cpp
         src/sg_determine_drivability_class.cpp
)

target_link_libraries(
   SG-Detection-Processing
      PRIVATE sg_compiler_config
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Common
      PRIVATE SG-Host
      PRIVATE SG-Math
      PRIVATE SG-Calibrations
      PRIVATE SG-Rspp
)

get_target_property(src_files SG-Detection-Processing SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${src_files})