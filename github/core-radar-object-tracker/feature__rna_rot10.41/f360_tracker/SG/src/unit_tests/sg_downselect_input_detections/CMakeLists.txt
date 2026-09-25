cmake_minimum_required(VERSION 3.16)

add_executable(sg_downselect_input_detections_ut)

target_sources(
   sg_downselect_input_detections_ut
      PRIVATE
         ut_sg_downselect_input_detections.cpp
)

target_link_libraries(
   sg_downselect_input_detections_ut
      PRIVATE sg_ut_config
      PRIVATE SG-Detection-Processing
      PRIVATE SG-Common
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Calibrations
)

target_include_directories(
   sg_downselect_input_detections_ut
      PRIVATE $<TARGET_PROPERTY:SG-Detection-Processing,INCLUDE_DIRECTORIES>
)

include(GoogleTest)
gtest_discover_tests(sg_downselect_input_detections_ut)
