cmake_minimum_required(VERSION 3.16)

add_executable(sg_measurement_update_ut)

target_sources(
   sg_measurement_update_ut
      PRIVATE
         ut_sg_measurement_update_contours.cpp
         ut_sg_length_measurement_update.cpp
         ut_sg_position_measurement_update.cpp
         ut_sg_measurement_update_utils.cpp
)

target_link_libraries(
   sg_measurement_update_ut
      PRIVATE sg_ut_config
      PRIVATE SG-Common
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Measurement-Update
      PRIVATE SG-Host
      PRIVATE SG-Math
      PRIVATE SG-Calibrations
)

target_include_directories(
   sg_measurement_update_ut
      PRIVATE $<TARGET_PROPERTY:SG-Measurement-Update,INCLUDE_DIRECTORIES>
)

include(GoogleTest)
gtest_discover_tests(sg_measurement_update_ut)
