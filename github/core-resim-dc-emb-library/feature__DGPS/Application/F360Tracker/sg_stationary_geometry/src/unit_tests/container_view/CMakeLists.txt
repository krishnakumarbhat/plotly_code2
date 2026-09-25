cmake_minimum_required(VERSION 3.16)

add_executable(container_view_ut)

target_sources(
   container_view_ut
      PRIVATE
         ut_container_view.cpp
)

target_link_libraries(
   container_view_ut
      PRIVATE sg_ut_config
      PRIVATE SG-Common
      PRIVATE stationary_geometries_iface
)

include(GoogleTest)
gtest_discover_tests(container_view_ut)
