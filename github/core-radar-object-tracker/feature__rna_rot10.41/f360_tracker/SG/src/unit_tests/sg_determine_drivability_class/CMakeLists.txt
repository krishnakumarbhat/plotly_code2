cmake_minimum_required(VERSION 3.16)

add_executable(sg_determine_drivability_class_ut)

target_sources(
   sg_determine_drivability_class_ut
      PRIVATE
         ut_sg_determine_drivability_class.cpp
)

target_link_libraries(
   sg_determine_drivability_class_ut
      PRIVATE sg_ut_config
      PRIVATE SG-Common
      PRIVATE SG-Detection-Processing
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Calibrations
)

include(GoogleTest)
gtest_discover_tests(sg_determine_drivability_class_ut)
