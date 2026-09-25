cmake_minimum_required(VERSION 3.16)

add_executable(sg_contour_downselection_ut)

target_sources(
   sg_contour_downselection_ut
      PRIVATE
         ut_sg_contour_downselection.cpp
)

target_link_libraries(
   sg_contour_downselection_ut
      PRIVATE sg_ut_config
      PRIVATE SG-Common
      PRIVATE stationary_geometries_iface
      PRIVATE SG-Contour-Downselection
      PRIVATE SG-Math
      PRIVATE SG-Host
      PRIVATE SG-Calibrations
)

target_include_directories(
   sg_contour_downselection_ut
      PRIVATE $<TARGET_PROPERTY:SG-Contour-Downselection,INCLUDE_DIRECTORIES>
)

include(GoogleTest)
gtest_discover_tests(sg_contour_downselection_ut)