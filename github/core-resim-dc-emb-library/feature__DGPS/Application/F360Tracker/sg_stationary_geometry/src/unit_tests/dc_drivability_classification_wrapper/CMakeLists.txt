cmake_minimum_required(VERSION 3.16)

add_executable(dc_drivability_classification_wrapper_ut)

target_sources(
   dc_drivability_classification_wrapper_ut
      PRIVATE
         ut_dc_tooling_wrapper.cpp
)

target_link_libraries(
   dc_drivability_classification_wrapper_ut
      PRIVATE DC-Drivability-Classification
      PRIVATE DC-Drivability-Classification-Wrapper
      PRIVATE stationary_geometries_iface
      PRIVATE sg_ut_config
)

include(GoogleTest)
gtest_discover_tests(dc_drivability_classification_wrapper_ut)
