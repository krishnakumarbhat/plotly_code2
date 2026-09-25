cmake_minimum_required(VERSION 3.16)

add_executable(calibrations_ut)

target_sources(
   calibrations_ut
      PRIVATE
         ut_sg_calibrations.cpp
         ut_calibration_manager.cpp
         ut_calibration_initializer.cpp
         sg_compare_calibrations.h
         sg_calibrations_ut_helpers.h
         sg_set_expected_calibrations_if.h
         sg_set_expected_calibrations_for_platform.h
         sg_set_expected_calibrations_for_platform_reduced.h
         sg_set_expected_calibrations_for_scania.h
         sg_check_calibrations_platform_high_parking.h
         sg_check_calibrations_platform_high_city.h
         sg_check_calibrations_platform_high_highway.h
         sg_check_calibrations_platform_medium_parking.h
         sg_check_calibrations_platform_medium_city.h
         sg_check_calibrations_platform_medium_highway.h
         sg_check_calibrations_platform_low_parking.h
         sg_check_calibrations_platform_low_city.h
         sg_check_calibrations_platform_low_highway.h
         sg_check_calibrations_platform_unlimited_parking.h
         sg_check_calibrations_platform_unlimited_city.h
         sg_check_calibrations_platform_unlimited_highway.h
         sg_check_calibrations_platform_reduced_parking.h
         sg_check_calibrations_platform_reduced_city.h
         sg_check_calibrations_platform_reduced_highway.h
         sg_check_calibrations_scania_parking.h
         sg_check_calibrations_scania_city.h
         sg_check_calibrations_scania_highway.h
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
