cmake_minimum_required(VERSION 3.16)

add_executable(sg_measurement_association_ut)

target_sources(
    sg_measurement_association_ut
      PRIVATE
         ut_sg_associate_detections_to_contour.cpp
         ut_sg_measurement_association_utils.cpp
)

target_link_libraries(
    sg_measurement_association_ut
      PRIVATE sg_ut_config
      PRIVATE SG-Measurement-Association
      PRIVATE SG-Common
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Dummy-Generator
      PRIVATE SG-Detection-Processing
      PRIVATE SG-Host
      PRIVATE SG-Math
      PRIVATE SG-Calibrations
)

target_include_directories(
    sg_measurement_association_ut
      PRIVATE $<TARGET_PROPERTY:SG-Measurement-Association,INCLUDE_DIRECTORIES>
)

include(GoogleTest)
gtest_discover_tests(sg_measurement_association_ut)
