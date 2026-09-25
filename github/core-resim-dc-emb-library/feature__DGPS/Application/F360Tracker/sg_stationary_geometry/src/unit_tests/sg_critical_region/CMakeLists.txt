cmake_minimum_required(VERSION 3.16)

add_executable(sg_critical_region_ut)

target_sources(
   sg_critical_region_ut
      PRIVATE
         ut_sg_create_critical_region.cpp
         ut_sg_extend_critical_region.cpp
         ut_sg_is_vertex_in_critical_region.cpp
)

target_link_libraries(
   sg_critical_region_ut
      PRIVATE SG-Calibrations
      PRIVATE SG-Common
      PRIVATE SG-Critical_Region
      PRIVATE SG-Math
      PRIVATE sg_ut_config
      PRIVATE stationary_geometries_iface
)

target_include_directories(
   sg_critical_region_ut
      PRIVATE $<TARGET_PROPERTY:SG-Critical_Region,INCLUDE_DIRECTORIES>
)

include(GoogleTest)
gtest_discover_tests(sg_critical_region_ut)
