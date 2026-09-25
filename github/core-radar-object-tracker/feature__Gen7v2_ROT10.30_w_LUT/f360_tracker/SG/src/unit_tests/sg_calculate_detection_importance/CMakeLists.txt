cmake_minimum_required(VERSION 3.16)

add_executable(sg_calculate_detection_importance_ut)

target_sources(
   sg_calculate_detection_importance_ut
      PRIVATE
         ut_sg_calculate_detection_importance.cpp
)

target_link_libraries(
   sg_calculate_detection_importance_ut
      PRIVATE sg_ut_config
      PRIVATE SG-Detection-Processing
      PRIVATE SG-Common
      PRIVATE stationary_geometries_iface
)

target_include_directories(
   sg_calculate_detection_importance_ut
      PRIVATE $<TARGET_PROPERTY:SG-Detection-Processing,INCLUDE_DIRECTORIES>
)

include(GoogleTest)
gtest_discover_tests(sg_calculate_detection_importance_ut)
