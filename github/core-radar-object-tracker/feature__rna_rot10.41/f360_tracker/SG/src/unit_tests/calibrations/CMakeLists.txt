cmake_minimum_required(VERSION 3.16)

add_executable(calibrations_ut)

target_sources(
   calibrations_ut
      PRIVATE
         ut_sg_calibrations.cpp
         ut_calibration_manager.cpp
)

target_link_libraries(
  calibrations_ut
    PRIVATE sg_ut_config
    PRIVATE stationary_geometries_iface
    PRIVATE SG-Math
    PRIVATE SG-Calibrations
)

target_include_directories(
   calibrations_ut
      PRIVATE $<TARGET_PROPERTY:SG-Calibrations,INCLUDE_DIRECTORIES>
)

include(GoogleTest)
gtest_discover_tests(calibrations_ut)
