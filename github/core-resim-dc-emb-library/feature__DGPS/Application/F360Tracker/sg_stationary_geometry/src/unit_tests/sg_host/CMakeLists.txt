cmake_minimum_required(VERSION 3.16)

add_executable(sg_host_props_ut)

target_sources(
   sg_host_props_ut
      PRIVATE
         ut_sg_host_props.cpp
)

target_link_libraries(
  sg_host_props_ut
    PRIVATE sg_ut_config
    PRIVATE SG-Host
    PRIVATE SG-Math
    PRIVATE SG-Rspp
)

include(GoogleTest)
gtest_discover_tests(sg_host_props_ut)
